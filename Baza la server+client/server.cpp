#include "libraries.h"

int main ()
{
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
      perror ("[server] Eroare la accept().\n");
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
          cout << "[server] Client wants to add an app. \n";

          char information[100];
          bzero(information,100);
          strcpy(information, "[testing] Application has been added successfully.");

          printf ("[server] Sending back information... \n");

          int bytes = strlen(information) + 1;  // bytes de trimis la client

          if (write (client, &bytes, sizeof(int)) <= 0)
          {
              perror ("[server] Error at writting num bytes for client.\n");
              break; /* continuam sa ascultam */
          }

          // trimitere comanda la server
          if (write (client, information, bytes) <= 0)
          {
              perror ("[server] Error at writting command for client.\n");
              break; /* continuam sa ascultam */
          }
          else
              printf("[server] Client has received the message.\n\n");
        }
        else
        if(command == "Search")
        {
          printf ("[server] Client wants to add an app. \n");

          char information[100];
          bzero(information,100);
          strcpy(information, "[testing] Applications have been found.");

          printf ("[server] Sending back information... \n");

          int bytes = strlen(information) + 1;  // bytes de trimis la client

          if (write (client, &bytes, sizeof(int)) <= 0)
          {
              perror ("[server] Error at writting num bytes for client.\n");
              break; /* continuam sa ascultam */
          }

          // trimitere comanda la server
          if (write (client, information, bytes) <= 0)
          {
              perror ("[server] Error at writting command for client.\n");
              break; /* continuam sa ascultam */
          }
          else
              printf("[server] Client has received the message.\n\n");
        }
      }
      /* am terminat cu acest client, inchidem conexiunea */
      close (client);
      exit(1);
    }
    // parent process or error at fork() :
    close(client);
    }
}
