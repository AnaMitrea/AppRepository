#include "libraries.h"

#define PORT 2024
#define MAXIMUM 1024

int errorHandling(string errmsg);
void sighandler();

int errorHandling(string errmsg)
{
  perror(errmsg.c_str());
  return errno;
}

void sighandler()
{
  while(waitpid(-1, NULL, WNOHANG) > 0);
}

int main ()
{
  struct sockaddr_in server;	// structura folosita de server
  struct sockaddr_in from;	
  int sd;			// socket descriptor

  if (signal (SIGCHLD, (sighandler_t)sighandler) == SIG_ERR)
  {
    perror ("signal()");
    return 1;
  }

  /* crearea unui socket */
  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
  {
    errorHandling("[server]Eroare la socket().\n");
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
    errorHandling("[server]Eroare la bind().\n");
  }

  /* punem serverul sa asculte daca vin clienti sa se conecteze */
  if (listen (sd, 5) == -1)
  {
    errorHandling("[server]Eroare la listen().\n");
  }

  /* servim in mod concurent clientii... */
  while (1)
  {
    int client;
    socklen_t length = sizeof (from);

    printf ("[server] Asteptam la portul %d...\n",PORT);
    fflush (stdout);

    /* acceptam un client (stare blocanta pana la realizarea conexiunii) */
    client = accept (sd, (struct sockaddr *) &from, &length);

    /* eroare la acceptarea conexiunii de la un client */
    if (client < 0)
    {
      perror ("[server] Eroare la accept().\n");
      continue;
    }

    if(fork() == 0)  // child process
    {
      close(sd);
      char command[MAXIMUM];

      while(1)
      {
        /* s-a realizat conexiunea, se astepta mesajul */
        bzero (command, MAXIMUM);
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

        if (read (client, command, bytes_sent) < 0)
        {
            perror ("[server] Error at reading command from client.\n");
            close(client);  /* inchidem conexiunea cu clientul */
            break;  /* continuam sa ascultam */
        }
        
        command[strlen(command)] = '\0';
        printf ("[server] Command sent by client: \"%s\"\n", command);
        

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
        if(strcmp(command,"Search") == 0)
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
