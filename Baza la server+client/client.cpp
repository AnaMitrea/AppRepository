#include "libraries.h"
int port;


int main (int argc, char *argv[])
{
  int sd;
  struct sockaddr_in server;
  string command;		// input command

  // ./client <server_address> <port>
  if (argc != 3)
  {
    printf ("Sintax: %s <server_address> <port>\n", argv[0]);
    return -1;
  }

  port = atoi (argv[2]); /* Stabilire port */

  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
  {
    errorHandling("[client] Error socket().\n");
    return 0;
  }

  // Filling the structure used for establishing connection with server
  server.sin_family = AF_INET; // Socket Family
  server.sin_addr.s_addr = inet_addr(argv[1]); // IP Address
  server.sin_port = htons (port); // Connection port
  
  if (connect (sd, (struct sockaddr *) &server,sizeof (struct sockaddr)) == -1)
  {
    errorHandling("[client] Error connect().\n");
    return 0;
  }

  while(1)
  {
    printInstructions();
    int ok = 1;

    command.clear();

    cout << "[client] Write your command: ";
    fflush (stdout);
    getline(cin, command);

    if(command == "Disconnect")
    {
      int bytes = strlen("Disconnect") + 1;
      sendingCommand_CLIENT(sd, bytes, command);

      cout << "You have disconnected from the server...\n"s;
      close(sd);
      return 0;
    }
    else
    if(command == "Insert")
    {
      string insertInfo; // holds information about sql query for inserting in db
      insertInfo.clear();

      int bytes = strlen("Insert") + 1;
      sendingCommand_CLIENT(sd, bytes, command);

      string name;
      cout << "Complete the following information about your application.\n";
      cout << "Application Name: "; 
      getline(cin, name);

      while(name.empty() == 1)
      {
          cout << "Application's Name field must be completed!" << endl;
          cout << "Application Name: ";
          getline(cin, name);
      }

      bytes = name.length() + 1;
      sendingCommand_CLIENT(sd, bytes, name); // sending name for verifying it (YES/NO)
      
      // reading the answer YES or NO
      int bytes_sent;
      if (read (sd, &bytes_sent, sizeof(int)) <= 0)
      {
        errorHandling("[ERROR] Error at reading num bytes from server.\n");
        close(sd);
        exit(1);
      }

      char answer[bytes_sent];
      bzero(answer, bytes_sent);

      if (read (sd, answer, bytes_sent) <= 0)
      {
        errorHandling("[ERROR] Error at reading message from server.\n");
        close(sd);
        exit(1);
      }

      if(strcmp(answer,"YES") == 0)
      {
        while(strcmp(answer,"YES") == 0) // name already exists?
        {
          name.clear();
          cout << "Project name already exists. Choose another name." << endl;
          cout << "Project Name: ";
          getline(cin, name);

          bytes = name.length() + 1;
          sendingCommand_CLIENT(sd, bytes, name); // sending name for verifying it
          
          // reading the answer YES or NO
          bytes_sent = -1;
          if (read (sd, &bytes_sent, sizeof(int)) <= 0)
          {
            errorHandling("[ERROR] Error at reading num bytes from server.\n");
            close(sd);
            exit(1);
          }

          char answer[bytes_sent];
          bzero(answer, bytes_sent);

          if (read (sd, answer, bytes_sent) <= 0)
          {
            errorHandling("[ERROR] Error at reading message from server.\n");
            close(sd);
            exit(1);
          }

          if(strcmp(answer,"NO") == 0)
          {
            break;
          }
        }
      }

      insertInfo.clear();
      insertInfo = insertValues_Application(name);

      bytes = insertInfo.length() + 1;
      sendingCommand_CLIENT(sd,bytes, insertInfo); // sending info from Application table
      
      name.clear();
      insertInfo.clear();

      string distro_name;
      string exec_name;
      distro_name.clear();
      exec_name.clear();

      cout << "\nOperating System details:\n";
      cout << "OS distribution: "; 
      getline(cin, distro_name);

      // trimitere nume distro
      if(distro_name.empty() == 1)
      {
        insertInfo = "NO_distro";
        bytes = insertInfo.length() + 1;
        sendingCommand_CLIENT(sd,bytes, insertInfo);
      }
      else
      {
        bytes = distro_name.length() + 1;
        sendingCommand_CLIENT(sd,bytes, distro_name); // distro_name

        // EXECUTABLE NAME
        cout << "Do you want to upload the executable of your application? Write YES or NO.\n";
        cout << "Your command: ";

        command.clear();
        getline(cin, command);
        if(command == "YES")
        { 
          bytes = command.length() + 1;
          sendingCommand_CLIENT(sd, bytes, command); // YES

          cout << "Executable (as\"example.exe\", \"example.app\" etc): ";
          exec_name.clear();
          getline(cin, exec_name);

          while(exec_name.empty() == 1)
          {
            cout << "Executable's Name field must be completed!" << endl;
            cout << "Executable: ";
            getline(cin, exec_name);
          }

          cout << "exec_name= " << exec_name << "|\n";
          bytes = exec_name.length() + 1;
          sendingCommand_CLIENT(sd, bytes, exec_name); // exec_name

          
          // sendFile_to_server()
        }
        else
        {
          bytes = command.length() + 1;
          sendingCommand_CLIENT(sd, bytes, command);
        }
      }

      cout << endl << insertInfo << endl;
      // sending the distro name + executable name
      bytes = insertInfo.length() + 1;
      sendingCommand_CLIENT(sd, bytes, insertInfo);
      insertInfo.clear();
      name.clear();
      
      cout << "To add one more distribution, write \"ADD\", otherwise write \"STOP\"\nYour command: ";
      getline(cin, name);
      if(name == "ADD")
      {
        bytes = name.length() + 1;
        sendingCommand_CLIENT(sd, bytes, name); // sending ADD
        name.clear();

        cout << "OS distribution: "; 
        getline(cin, name);

        // trimitere nume distro
        if(name.empty() == 1)
        {
          insertInfo = insertInfo + "\"-\"";
        }
        else
        {
          insertInfo = insertInfo + "\""+ name + "\"";
        }

        // sending the distro name
        bytes = insertInfo.length() + 1;
        sendingCommand_CLIENT(sd, bytes, insertInfo);
        insertInfo.clear();
        name.clear();
      }
      else
      {
        bytes = name.length() + 1;
        sendingCommand_CLIENT(sd, bytes, name); // sending STOP
        name.clear();
      }

      cout << "Minimum requirements for the application:\n";
      insertInfo.clear();
      insertInfo = insertValues_Minimum_Req();

      bytes = insertInfo.length() + 1;
      sendingCommand_CLIENT(sd,bytes, insertInfo); // sending info from Minimum_Req table
      insertInfo.clear();

      string check_read;
      check_read.clear();
      check_read = readingInfo_CLIENT(sd); //Application inserted.
      if(check_read == "ERROR!")
      {
        errorHandling("[ERROR] Error at reading message from server.\n");
        close(sd);
        exit(1);
      }
      cout << check_read << endl;
    }
    else
    if(command == "Search")
    {
      int bytes = strlen("Search") + 1;
      sendingCommand_CLIENT(sd, bytes, command); // sending written command

      string availableCriteria = "\nCriteria available:\n- App's Name\n- Developer\n- License\n- Category\n- Internet Connection\n- Operating System\n- CPU\n- RAM\n- Hard Disk Storage\n\n";
      cout << availableCriteria;

      string searchInfo; // holds information about sql query for searching db with criteria
      searchInfo.clear();
      searchInfo = searchApps();

      bytes = searchInfo.length() + 1;
      sendingCommand_CLIENT(sd, bytes, searchInfo); // sending information about the search criteria

      string check_read;
      check_read.clear();
      check_read = readingInfo_CLIENT(sd); // reading the number of apps found
      if(check_read == "ERROR!")
      {
        errorHandling("[ERROR] Error at reading message from server.\n");
        close(sd);
        exit(1);
      }
      cout << check_read << endl;

      check_read.clear();
      check_read = readingInfo_CLIENT(sd); // reading all the apps
      if(check_read == "ERROR!")
      {
        errorHandling("[ERROR] Error at reading message from server.\n");
        close(sd);
        exit(1);
      }
      cout << check_read << endl;

      cout << "[client] To download an application, write down the ID_exec from the list, otherwise, write STOP.\n";
      cout << "ID_exec: ";
      fflush (stdout);
      command.clear();
      getline(cin, command);

      if(command.empty() == 1 || command == "STOP")
      {
        bytes = command.length() + 1;
        sendingCommand_CLIENT(sd, bytes, command);
      }
      else // s a introdus un id pt executabilul respectiv
      {
        bytes = command.length() + 1;
        sendingCommand_CLIENT(sd, bytes, command);
        // serverul verifica .....


        check_read.clear();
        check_read = readingInfo_CLIENT(sd); // ok sau not ok in functie de corectitudinea id_exec
        if(check_read == "ERROR!")
        {
          errorHandling("[ERROR] Error at reading message from server.\n");
          close(sd);
          exit(1);
        }
        else
        if(check_read == "UNKNOWN") // Id ul introdus este gresit
        {
          cout << "Error at downloading app. Unknown ID_exec. Please try again." << endl;
        }
        else
        if(check_read == "OK")  // id ul introdus este corect si executabilul poate fi descarcat
        {
          // ...
        }

      }


      /*
      cout << "Vr sa descarci o aplicatie?" << endl;
      -> ID/NU/empty()

      receive_file(fd);
      */
    }
    else
    {
      if(command.empty() == 0)
      {
        cout << endl << "\""<< command << "\" is an unavailable command. \n";
      }
      else
      {
        cout << "\nType a command!";
      }
      ok = 0;
    }
  }
}
