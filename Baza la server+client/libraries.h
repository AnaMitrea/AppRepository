#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <string>
#include <iostream>

using namespace std;


// CLIENT+SERVER FUNCTIONS
#define MAXIMUM 1024



// CLIENT FUNCTIONS
int errorHandling(string errmsg);
void printInstructions();
void readingInfo_IN_CLIENT(int sd);
void sendingCommand_IN_CLIENT(int sd, int bytes, string command);


// SERVER FUNCTIONS
#define PORT 2024
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

void printInstructions()
{
  cout << "\nAvailable commands on the server: \n";
  cout << "[1] To insert your Application, please write \"Insert\". \n";
  cout << "[2] To search an Application, please write \"Search\". \n";
  cout << "[3] To Disconnect from the server, please write \"Disconnect\". \n" << endl;
}

void sendingCommand_IN_CLIENT(int sd, int bytes, string command)
{
  if (write (sd, &bytes, sizeof(int)) <= 0) /// trimitere bytes la server 
  {
    errorHandling("[client] Error at writting num bytes for server.\n");
  }
  
  if (write (sd, command.c_str(), bytes) <= 0)  // trimitere comanda la server
  {
    errorHandling("[client] Error at writting command for server.\n");
  }
}

void readingInfo_IN_CLIENT(int sd)
{
  int bytes_sent;
  // citirea raspunsului dat de server (apel blocant pana cand serverul raspunde)
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

  cout << "[client] " << information << endl;
}
