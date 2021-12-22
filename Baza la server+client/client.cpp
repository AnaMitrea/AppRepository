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
  }

  // Filling the structure used for establishing connection with server
  server.sin_family = AF_INET; // Socket Family
  server.sin_addr.s_addr = inet_addr(argv[1]); // IP Address
  server.sin_port = htons (port); // Connection port
  
  if (connect (sd, (struct sockaddr *) &server,sizeof (struct sockaddr)) == -1)
  {
    errorHandling("[client] Error connect().\n");
  }

  printInstructions();

  while(1)
  {
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
      int bytes = strlen("Insert") + 1;
      sendingCommand_CLIENT(sd, bytes, command);
    }
    else
    if(command == "Search")
    {
      int bytes = strlen("Search") + 1;
      sendingCommand_CLIENT(sd, bytes, command);
    }
    else
    {
        cout << endl << command << " is an unavailable command. \n";
        printInstructions();
        ok = 0;
    }

    if(ok == 1)
    {
      readingInfo_CLIENT(sd);
    }
  }
}
