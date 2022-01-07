#include "libraries.h"

int main ()
{
  sqlite3* db;
  int database = sqlite3_open("SoftwareApp.db", &db);

  if (database)
  {
      cout << "Error: " << sqlite3_errmsg (db) << endl;
      return 0;
  }
  else
  {
      cout << "Database connected succesfully...\n";
  }

  string searchInfo; // holds information about sql query for searching db with criteria
  string insertInfo; // holds information about sql query for inserting in db
  string sqlQuery; // sql query to be executed
  string sqlResponse; // sql query response

  struct sockaddr_in server;	// structure used by server
  struct sockaddr_in from;	
  int sd;

  if (signal (SIGCHLD, (sighandler_t)sighandler) == SIG_ERR)
  {
    errorHandling("[server]Error signal().\n");
    return 0;
  }

  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
  {
    errorHandling("[server]Error socket().\n");
    return 0;
  }

  bzero (&server, sizeof (server));
  bzero (&from, sizeof (from));
  
  // Filling the structure used for establishing connection with server
  server.sin_family = AF_INET;	// Socket Family
  server.sin_addr.s_addr = htonl (INADDR_ANY); // any address
  server.sin_port = htons (PORT); // Connection port
  
  int enable = 1;
  if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
  {
    errorHandling("setsockopt(SO_REUSEADDR) failed.\n");
    return 0;
  }

  if (bind (sd, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)
  {
    errorHandling("[server]Error bind().\n");
    return 0;
  }

  if (listen (sd, 5) == -1) // queue of 5 
  {
    errorHandling("[server]Error listen().\n");
    return 0;
  }

  cout << "[server] Waiting at "<< PORT << " port...\n";
  fflush (stdout);

  while (1)
  {
    int client;
    socklen_t length = sizeof (from);

    // Accepting client (blocking state till connection establishes)
    client = accept (sd, (struct sockaddr *) &from, &length);

    if (client < 0)
    {
      cout << "[server] Erorr accept().\n";
      continue;
    }

    if(fork() == 0)  // child process
    {
      close(sd);

      while(1)
      {
        // connection ok, waiting msg
        cout << "[server] Waiting client to write command...\n";
        fflush (stdout);
        string command = readingCommand_SERVER(client);
        if(command == "ERROR!")
        {
          printf ("[server] A client has lost connection from the server. \n");
          close (client); 
          exit(1);
        }
        
        cout << "[server] Command sent by client: \"" << command << "\"\n";

// DISCONNECT COMMAND
        if(command == "Disconnect")
        {
          printf ("[server] A client has disconnected from server. \n");
          close (client); 
          exit(1);
        }
// INSERT COMMAND
        else
        if(command == "Insert")
        {
          cout << "[server] Client wants to add an app in the database. \n";
          insertInfo.clear();

          string name = readingCommand_SERVER(client); // reading the appname
          if(name == "ERROR!")
          {
            printf ("[server] A client has lost connection from the server. \n");
            close (client); 
            exit(1);
          }

          bool verif = verifyingExistingName(db, name);
          if(verif == true)
          {
            while(verif == true)
            {
              cout << "The \"" << name << "\" is already in the database.\n";
              cout << "[server] Sending back information... \n";
              sendingInfo_SERVER(client, "YES");

              name.clear();
              name = readingCommand_SERVER(client); // reading the appname
              if(name == "ERROR!")
              {
                printf ("[server] A client has lost connection from the server. \n");
                close (client); 
                exit(1);
              }

              verif = verifyingExistingName(db, name);
              if(verif == false)
              {
                cout << "[server] Sending back information... \n";
                sendingInfo_SERVER(client, "NO");
                break;
              }
            }
          }
          else
          {
            sendingInfo_SERVER(client, "NO");
          }
          string appName = name;

    // inserting in Application Table
          insertInfo = readingCommand_SERVER(client); // info from application table
          if(insertInfo == "ERROR!")
          {
            printf ("[server] A client has lost connection from the server. \n");
            close (client); 
            exit(1);
          }

          sqlQuery.clear();

          sqlQuery = "INSERT INTO Application(AppName, Developer, License, Category, InternetConnection, AppInfo) VALUES(" + insertInfo + ");";
          cout << "Application table - sqlQuery: " << sqlQuery << endl;
          sqlResponse = insertQuery(db, sqlQuery);
          cout << endl << sqlResponse << endl; // Inserting Query succeeded or not
          sqlResponse.clear();
          sqlQuery.clear();
cout << "dupa inserare in app table" << endl;
    // Inserting in Minimum_req table
          string appID = getAppID(db,appName);
          insertInfo.clear();
          sqlQuery.clear();
          sqlResponse.clear();

          insertInfo.clear();
          insertInfo = readingCommand_SERVER(client); // info to be inserted in Minimum_Req Table
          if(insertInfo == "ERROR!")
          {
            printf ("[server] A client has lost connection from the server. \n");
            close (client); 
            exit(1);
          }
          cout << "Min_req table - sqlQuery: " << insertInfo << endl;
          sqlQuery = "INSERT INTO Minimum_Req(AppID,GHzCPU, GPU, GB_RAM, GB_HDStorage) VALUES(" + appID + "," + insertInfo + ");";
          sqlResponse = insertQuery(db, sqlQuery);
          cout << endl << sqlResponse << endl; // Inserting Query succeeded or not
          sqlResponse.clear();
cout << "dupa inserare in min_req" << endl;
    // inserting in OS Table
          string distro_name;
          distro_name.clear();

          distro_name = readingCommand_SERVER(client); // OS_name to be inserted in OS table
          cout << "distro_name=" << distro_name << endl;

          if(distro_name == "ERROR!")
          {
            printf ("[server] A client has lost connection from the server. \n");
            close (client); 
            exit(1);
          }

          if(distro_name != "NO_distro") // client inserted a distro_name
          {
            string maxID_exec = getMAXID_exec(db);

            command.clear();
            command = readingCommand_SERVER(client); // yes or no or error
            if(command == "ERROR!")
            {
              printf ("[server] A client has lost connection from the server. \n");
              close (client); 
              exit(1);
            }

            if(command == "YES")
            {
              // yes inseamna ca clientul vrea sa faca upload la fisierul cu numele din distro_name
              // dar in tabel trebuie salvat ca si apps/"maxID_exec".extensie
              // de verificat daca nume.extensie exista in folder
              
              string exec_name;  // name.extension
              exec_name.clear();
              exec_name = readingCommand_SERVER(client); // exec_name such as MyApp.exe
              cout << "exec_name="<< exec_name << endl;
              if(exec_name == "ERROR!")
              {
                printf ("[server] A client has lost connection from the server. \n");
                close (client); 
                exit(1);
              }
              if(exec_name != "FILE_DOES_NOT_EXIST")
              {
                string extension; // .extension from exec_name
                extension.clear();

                int pos = exec_name.find(".");
                extension = exec_name.substr(pos,string::npos);

                string execInsert;  //executable_name to be inserted in OS table as apps/id.extension
                execInsert.clear();
                execInsert = "apps/" + maxID_exec + extension;

                sqlQuery = "INSERT INTO OS(AppID, OS_Name, ID_exec, Executable_Name) VALUES("+ appID + ",\"" + distro_name + "\"," +  maxID_exec + ",\"" + execInsert + "\");";
                cout << "OS table - sqlQuery: " << sqlQuery << endl;
                sqlResponse = insertQuery(db, sqlQuery);
                cout << endl << sqlResponse << endl; // Inserting Query succeeded or not
                sqlResponse.clear();

                sqlResponse = "\nApplication inserted.";
                cout << "[server] Sending back information... \n";
                sendingInfo_SERVER(client, sqlResponse);
              }
            }
          }
        }
// SEARCH COMMAND
        else
        if(command == "Search")
        {
          cout << "[server] Client wants to search an app. \n";

          sqlQuery.clear();
          searchInfo.clear();

          searchInfo = readingCommand_SERVER(client); // search criteria
          if(searchInfo == "ERROR!")
          {
            printf ("[server] A client has lost connection from the server. \n");
            close (client); 
            exit(1);
          }

          cout << "Search criteria: \"" <<  searchInfo << "\"." << endl;
          
          if(searchInfo.empty() == 0) // nu e empty
          {
            sqlQuery = "SELECT COUNT(DISTINCT AppName) FROM Application LEFT JOIN OS USING(AppID) LEFT JOIN Minimum_Req USING(AppID) WHERE " + searchInfo + ";";

            string appsFound = numberOfAppsFound(db, sqlQuery);
            string found = "[Found " + appsFound + " programs for the written criteria]";
            sendingInfo_SERVER(client, found); // sending the number of apps found

            if(appsFound != "0")
            {
              sqlQuery.clear();
              sqlQuery = "SELECT * FROM Application LEFT JOIN OS USING(AppID) LEFT JOIN Minimum_Req USING(AppID) WHERE " + searchInfo + ";";
              sqlResponse = selectQuery_SEARCH(db, sqlQuery);
              sendingInfo_SERVER(client, sqlResponse); //sending all the apps which have the criteria


              // clientul trimite STOP sau ID_exec
              command.clear();
              command = readingCommand_SERVER(client);
              if(command == "ERROR!")
              {
                printf ("[server] A client has lost connection from the server. \n");
                close (client); 
                exit(1);
              }

              if(command != "STOP") // inseamna ca s a introdus un id pt download aplicatie si trebuie verificat daca exista
              {
                // caca

              }

              /*
              citire raspuns 

              msg != nu 

                idkit

                send_file(fd, idkit)
              */
            }
            else
            {
              sendingInfo_SERVER(client, "Try entering other criteria.");
            }
          }
          else
          {
            sendingInfo_SERVER(client, "Found 0 programs.");
            sendingInfo_SERVER(client, "No criteria entered.");
          }
          
         
          sqlQuery.clear();
          sqlResponse.clear();
          searchInfo.clear();
        }
      }
      close (client);
      exit(1);
    }
    // parent process or error at fork() :
    close(client);
    }
}
