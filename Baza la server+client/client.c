#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>

#define MAXIMUM 1024
extern int errno; //codul de eroare returnat de anumite apeluri
int port;
extern int errorHandling(char* errmsg);
void printInstructions();
void sendingCommand2Server(int sd, int bytes, char command[MAXIMUM]);

extern int errorHandling(char* errmsg)
{
  perror(errmsg);
  return errno;
}

void printInstructions()
{
  printf("\n");
  printf("Available commands on the server: \n");
  printf("[1] To insert your Application, please write \"Insert\". \n");
  printf("[2] To search an Application, please write \"Search\". \n");
  printf("[3] To Disconnect from the server, please write \"Disconnect\". \n");
  printf("\n");
}

void sendingCommand2Server(int sd, int bytes, char command[MAXIMUM])
{
  if (write (sd, &bytes, sizeof(int)) <= 0) /// trimitere bytes la server 
  {
    errorHandling("[client] Error at writting num bytes for server.\n");
  }
  
  if (write (sd, command, bytes) <= 0)  // trimitere comanda la server
  {
    errorHandling("[client] Error at writting command for server.\n");
  }
}

int main (int argc, char *argv[])
{
  int sd;			// descriptor socket
  struct sockaddr_in server;	// structura folosita pentru conectare 
  char command[MAXIMUM];		// comanda introdusa

  /* Verificare existenta argumente in linia de comanda */
  if (argc != 3)
    {
      printf ("Sintaxa: %s <adresa_server> <port>\n", argv[0]);
      return -1;
    }

  /* Stabilire port */
  port = atoi (argv[2]);

  /* Creare socket */
  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
  {
    errorHandling("[client] Error socket().\n");
  }

  /* Umplere structura folosita pt realizarea conexiunii cu serverul */
  server.sin_family = AF_INET; /* Familia socket-ului */
  server.sin_addr.s_addr = inet_addr(argv[1]); /* Adresa IP a serverului */
  server.sin_port = htons (port); /* Portul de conectare */
  
  /* Conectare client la server */
  if (connect (sd, (struct sockaddr *) &server,sizeof (struct sockaddr)) == -1)
  {
    errorHandling("[client] Error connect().\n");
  }

  printInstructions();

  while(1)
  {
    int ok = 1;
    /* Citire comanda introdusa */
    bzero (command, MAXIMUM);
    printf ("[client] Write your command: ");
    fflush (stdout);
    read (0, command, MAXIMUM);

    command[strlen(command) - 1] = '\0';

    // Trimitere comanda introdusa catre server
    if(strcmp(command,"Disconnect") == 0)
    {
      int bytes = strlen("Disconnect") + 1;
      sendingCommand2Server(sd, bytes, command);

      printf("You have disconnected from the server...\n");
      close(sd);
      return 0;
    }
    else
    if(strcmp(command,"Insert") == 0)
    {
      int bytes = strlen("Insert") + 1;
      sendingCommand2Server(sd, bytes, command);
    }
    else
    if(strcmp(command,"Search") == 0)
    {
      int bytes = strlen("Search") + 1;
      sendingCommand2Server(sd, bytes, command);
    }
    else
    {
        printf("\n%s is an unavailable command. \n", command);
        printInstructions();
        ok = 0;
    }

    if(ok == 1)
    {
      int bytes_sent;
      /* citirea raspunsului dat de server (apel blocant pana cand serverul raspunde) */
      if (read (sd, &bytes_sent, sizeof(int)) < 0)
      {
        errorHandling("[client] Error at reading num bytes from server.\n");
      }

      char information[bytes_sent];
      bzero(information, bytes_sent);

      if (read (sd, information, bytes_sent) < 0)
      {
        errorHandling("[client] Error at reading message from server.\n");
      }

      information[strlen(information)] = '\0';

      /* afisam mesajul primit */
      printf ("[client] %s\n", information);
    }
  }
}
