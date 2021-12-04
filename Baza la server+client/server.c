#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>

#define PORT 2024

extern int errno; // codul de eroare returnat de anumite apeluri

void sighandler()
{
  while(waitpid(-1, NULL, WNOHANG) > 0);
}

int main ()
{
  struct sockaddr_in server;	// structura folosita de server
  struct sockaddr_in from;	
  int sd;			//descriptorul de socket 

  /*
  char command[100];		//mesajul primit de la client 
  char information[100]=" ";        //mesaj de raspuns pentru client
  */

  if (signal (SIGCHLD, sighandler) == SIG_ERR)
  {
    perror ("signal()");
    return 1;
  }

  /* crearea unui socket */
  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("[server]Eroare la socket().\n");
      return errno;
    }

  /* pregatirea structurilor de date */
  bzero (&server, sizeof (server));
  bzero (&from, sizeof (from));
  
  /* umplem structura folosita de server */
  /* stabilirea familiei de socket-uri */
    server.sin_family = AF_INET;	
  /* acceptam orice adresa */
    server.sin_addr.s_addr = htonl (INADDR_ANY);
  /* utilizam un port utilizator */
    server.sin_port = htons (PORT);
  
  /* atasam socketul */
  if (bind (sd, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)
    {
      perror ("[server]Eroare la bind().\n");
      return errno;
    }

  /* punem serverul sa asculte daca vin clienti sa se conecteze */
  if (listen (sd, 5) == -1)
    {
      perror ("[server]Eroare la listen().\n");
      return errno;
    }

  /* servim in mod iterativ clientii... */
  while (1)
  {
    int client;
    int length = sizeof (from);

    printf ("[server] Asteptam la portul %d...\n",PORT);
    fflush (stdout);

    /* acceptam un client (stare blocanta pina la realizarea conexiunii) */
    client = accept (sd, (struct sockaddr *) &from, &length);

    /* eroare la acceptarea conexiunii de la un client */
    if (client < 0)
    {
      perror ("[server] Eroare la accept().\n");
      continue;
    }


    if(fork() == 0)
    {
      close(sd);
      char command[100];

      while(1)
      {
        /* s-a realizat conexiunea, se astepta mesajul */
        bzero (command, 100);
        printf ("[server] Waiting client to write command...\n");
        fflush (stdout);
        
        /* Citire bytes si comanda de la client*/
        int bytes_sent;  //bytes trimisi de client
        if (read (client, &bytes_sent, sizeof(int)) < 0) 
        {
            perror ("[server] Error at reading num bytes from client.\n");
            close(client); /* inchidem conexiunea cu clientul */
            break;  /* continuam sa ascultam */
        }

        sleep(1);

        if (read (client, command, bytes_sent) < 0)
        {
            perror ("[server] Error at reading command from client.\n");
            close(client);  /* inchidem conexiunea cu clientul */
            break;  /* continuam sa ascultam */
        }
        
        command[strlen(command)] = '\0';
        printf ("[server] Bytes received: %d and Command received...%s\n", bytes_sent, command);
        

        /*pregatim mesajul de raspuns */
        if(strcmp(command,"Disconnect") == 0)
        {
          printf ("[server] A client has disconnected from server. \n");
          close (client); /* inchidem conexiunea cu clientul */
          exit(1);
        }
        else
        if(strcmp(command,"Insert") == 0)
        {
          printf ("[server] Client wants to add an app. \n");

          char information[100];
          bzero(information,100);
          strcpy(information, "Application has been added successfully.");

          printf ("[server] Sending back information... \n");

          int bytes = strlen(information) + 1;  // bytes de trimis la client

          if (write (client, &bytes, sizeof(int)) <= 0)
          {
              perror ("[server] Error at writting num bytes for client.\n");
              break; /* continuam sa ascultam */
          }

          sleep(1);

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
        if(strcmp(command,"Search") == 0)
        {
          printf ("[server] Client wants to add an app. \n");

          char information[100];
          bzero(information,100);
          strcpy(information, "Applications have been found.");

          printf ("[server] Sending back information... \n");

          int bytes = strlen(information) + 1;  // bytes de trimis la client

          if (write (client, &bytes, sizeof(int)) <= 0)
          {
              perror ("[server] Error at writting num bytes for client.\n");
              break; /* continuam sa ascultam */
          }

          sleep(1);

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
    close(client);
    }

}
