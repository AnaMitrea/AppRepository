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
    perror ("signal()");
    return 1;
  }

  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
  {
    errorHandling("[server]Error socket().\n");
  }

  bzero (&server, sizeof (server));
  bzero (&from, sizeof (from));
  
  // Filling the structure used for establishing connection with server
  server.sin_family = AF_INET;	// Socket Family
  server.sin_addr.s_addr = htonl (INADDR_ANY); // any address
  server.sin_port = htons (PORT); // Connection port
  
  if (bind (sd, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)
  {
    errorHandling("[server]Error bind().\n");
  }

  if (listen (sd, 5) == -1) // queue of 5 
  {
    errorHandling("[server]Error listen().\n");
  }

  while (1)
  {
    int client;
    socklen_t length = sizeof (from);

    cout << "[server] Waiting at "<< PORT << " port...\n";
    fflush (stdout);

    // Accepting client (blocking state till connection establishes)
    client = accept (sd, (struct sockaddr *) &from, &length);

    if (client < 0)
    {
      perror ("[server] Erorr accept().\n");
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
        // COMANDA PE CARE O TRIMITE CLIENTUL 
        string command = readingCommand_SERVER(client);
        
        cout << "[server] Command sent by client: \"" << command << "\"\n";
        
        if(command == "Disconnect")
        {
          printf ("[server] A client has disconnected from server. \n");
          close (client); 
          exit(1);
        }
        else
        if(command == "Insert")
        {
          cout << "[server] Client wants to add an app in the database. \n";

          string information;
          information.clear();
          information = "[testing] Application has been added successfully.";

          cout << "[server] Sending back information... \n";

          sendingInfo_SERVER(client, information);
        }
        else
        if(command == "Search")
        {
          cout << "[server] Client wants to search an app. \n";

          sqlQuery.clear();
          searchInfo.clear();

          searchInfo = readingCommand_SERVER(client); // search criteria
          sqlQuery = "SELECT COUNT(DISTINCT AppName) FROM Application LEFT JOIN OS USING(AppID) LEFT JOIN Minimum_Req USING(AppID) WHERE " + searchInfo + ";";

          string appsFound = numberOfAppsFound(db, sqlQuery);
          string found = "Found " + appsFound + " programs for the criteria. \n\n";
          sendingInfo_SERVER(client, found); // sending the number of apps found

          sqlQuery.clear();
          sqlQuery = "SELECT * FROM Application LEFT JOIN OS USING(AppID) LEFT JOIN Minimum_Req USING(AppID) WHERE " + searchInfo + ";";
          sqlResponse = selectQuery_SEARCH(db, sqlQuery);
          sendingInfo_SERVER(client, sqlResponse); //sending all the apps which have the criteria

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
