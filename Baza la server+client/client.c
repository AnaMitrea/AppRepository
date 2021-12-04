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

extern int errno; //codul de eroare returnat de anumite apeluri

int port; 

int main (int argc, char *argv[])
{
  int sd;			// descriptor socket
  struct sockaddr_in server;	// structura folosita pentru conectare 
  char command[100];		// comanda introdusa

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
      perror ("[client] Error socket().\n");
      return errno;
    }

  /* Umplere structura folosita pt realizarea conexiunii cu serverul */
  /* Familia socket-ului */
  server.sin_family = AF_INET;
  /* Adresa IP a serverului */
  server.sin_addr.s_addr = inet_addr(argv[1]);
  /* Portul de conectare */
  server.sin_port = htons (port);
  
  /* Conectare client la server */
  if (connect (sd, (struct sockaddr *) &server,sizeof (struct sockaddr)) == -1)
    {
      perror ("[client] Error connect().\n");
      return errno;
    }

  printf("\n");
  printf("Available commands on the server: \n");
  printf("[1] To insert your Application, please write \"Insert\". \n");
  printf("[2] To search an Application, please write \"Search\". \n");
  printf("[3] To Disconnect from the server, please write \"Disconnect\" \n");
  printf("\n");

  while(1)
  {
    int ok = 1;
    /* Citire comanda introdusa */
    bzero (command, 100);
    printf ("[client] Write your command: ");
    fflush (stdout);
    read (0, command, 100);

    command[strlen(command) - 1] = '\0';

    // Trimitere comanda introdusa catre server
    if(strcmp(command,"Disconnect") == 0)
    {
      /// trimitere bytes la server 
      int bytes = strlen("Disconnect") + 1;

      if (write (sd, &bytes, sizeof(int)) <= 0)
      {
          perror ("[client] Error at writting num bytes for server.\n");
          return errno;
      }

      sleep(1);

      // trimitere comanda la server
      if (write (sd, command, bytes) <= 0)
      {
          perror ("[client] Error at writting command for server.\n");
          return errno;
      }

      printf("You have disconnected from the server...\n");

      close(sd);
      return 0;
    }
    else
    if(strcmp(command,"Insert") == 0)
    {
      // trimitere bytes la server
      int bytes = strlen("Insert") + 1;

      if (write (sd, &bytes, sizeof(int)) <= 0)
      {
          perror ("[client] Error at writting num bytes for server.\n");
          return errno;
      }

      sleep(1);

      // trimitere comanda catre server
      if (write (sd, command, bytes) <= 0)
      {
          perror ("[client] Error at writting command for server.\n");
          return errno;
      }
    }
    else
    if(strcmp(command,"Search") == 0)
    {
      // trimitere bytes la server
      int bytes = strlen("Search") + 1;

      if (write (sd, &bytes, sizeof(int)) <= 0)
      {
          perror ("[client] Error at writting num bytes for server.\n");
          return errno;
      }

      sleep(1);

      if (write (sd, command, bytes) <= 0)
      {
        perror ("[client] Error at writting message for server.\n");
        return errno;
      }
    }
    else
    {
        printf("\n%s is an unavailable command. \n", command);
        printf("Available commands on the server: \n");
        printf("[1] To insert your Application, please write \"Insert\". \n");
        printf("[2] To search an Application, please write \"Search\". \n");
        printf("[3] To Disconnect from the server, please write \"Disconnect\" \n");
        printf("\n");
        ok = 0;
    }

    if(ok == 1)
    {
      int bytes_sent;
      /* citirea raspunsului dat de server (apel blocant pana cand serverul raspunde) */
      if (read (sd, &bytes_sent, sizeof(int)) < 0)
      {
        perror ("[client] Error at reading num bytes from server.\n");
        return errno;
      }

      sleep(1);

      char information[bytes_sent];
      bzero(information, bytes_sent);

      if (read (sd, information, bytes_sent) < 0)
      {
        perror ("[client] Error at reading message from server.\n");
        return errno;
      }

      information[strlen(information)] = '\0';

      /* afisam mesajul primit */
      printf("[client] num bytes: %d\n", bytes_sent);
      printf ("[client] Message: \n%s\n", information);
    }
  }
}
