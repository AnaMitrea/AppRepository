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

int port; // portul de conectare la server

int main (int argc, char *argv[])
{
  int sd;			// descriptorul de socket
  struct sockaddr_in server;	// structura folosita pentru conectare 
  char msg[100];		// mesajul trimis

  /* exista toate argumentele in linia de comanda? */
  if (argc != 3)
    {
      printf ("Sintaxa: %s <adresa_server> <port>\n", argv[0]);
      return -1;
    }

  /* stabilim portul */
  port = atoi (argv[2]);

  /* cream socketul */
  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("Eroare la socket().\n");
      return errno;
    }

  /* umplem structura folosita pentru realizarea conexiunii cu serverul */
  /* familia socket-ului */
  server.sin_family = AF_INET;
  /* adresa IP a serverului */
  server.sin_addr.s_addr = inet_addr(argv[1]);
  /* portul de conectare */
  server.sin_port = htons (port);
  
  /* ne conectam la server */
  if (connect (sd, (struct sockaddr *) &server,sizeof (struct sockaddr)) == -1)
    {
      perror ("[client]Eroare la connect().\n");
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
    /* citirea mesajului */
    bzero (msg, 100);
    printf ("[client] Write your command: ");
    fflush (stdout);
    read (0, msg, 100);

    msg[strlen(msg)-1]='\0';

    if(strcmp(msg,"Disconnect") == 0)
    {
        /* trimiterea mesajului la server */
      if (write (sd, "Disconnect", strlen("Disconnect")) <= 0)
      {
        perror ("[client] Error at writting message for server.\n");
        return errno;
      }

      printf("You have disconnected from the server...\n");
      break;
    }
    else
    if(strcmp(msg,"Insert") == 0)
    {
      if (write (sd, "Insert", strlen("Insert")) <= 0)
      {
        perror ("[client] Error at writting message for server.\n");
        return errno;
      }
    }
    else
    if(strcmp(msg,"Search") == 0)
    {
      if (write (sd, "Search", strlen("Search")) <= 0)
      {
        perror ("[client] Error at writting message for server.\n");
        return errno;
      }
    }
    else
    {
        printf("%s is an unavailable command. \n", msg);
        printf("Available commands on the server: \n");
        printf("[1] To insert your Application, please write \"Insert\". \n");
        printf("[2] To search an Application, please write \"Search\". \n");
        printf("[3] To Disconnect from the server, please write \"Disconnect\" \n");
        printf("\n");
    }


    /* citirea raspunsului dat de server (apel blocant pana cand serverul raspunde) */
    if (read (sd, msg, 100) < 0)
    {
      perror ("[client] Error at reading message from server.\n");
      return errno;
    }

    /* afisam mesajul primit */
    printf ("[client] Message: \n * %s\n", msg);

  }

  /* inchidem conexiunea, am terminat */
  close (sd);
}
