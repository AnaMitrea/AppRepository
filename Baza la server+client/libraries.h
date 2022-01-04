#pragma once
#include "database.h"

#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <signal.h>

// CLIENT FUNCTIONS
void errorHandling(string errmsg);
void printInstructions();
string readingInfo_CLIENT(int sd);
void sendingCommand_CLIENT(int sd, int bytes, string command);

// SERVER FUNCTIONS
#define PORT 2024
void sighandler();
string readingCommand_SERVER(int client);
void sendingInfo_SERVER(int sd);
int numBytesSent(int client);


void errorHandling(string errmsg)
{
  cout << errmsg.c_str();
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

void sendingCommand_CLIENT(int sd, int bytes, string command)
{
  if (write (sd, &bytes, sizeof(int)) <= 0)
  {
    errorHandling("[ERROR] Error at writting num bytes for server.\n");
  }
  
  if (write (sd, command.c_str(), bytes) <= 0)
  {
    errorHandling("[ERROR] Error at writting command for server.\n");
  }
}

string readingInfo_CLIENT(int sd)
{
  int bytes_sent;
  if (read (sd, &bytes_sent, sizeof(int)) <= 0)
  {
    errorHandling("[ERROR] Error at reading num bytes from server.\n");
    return "ERROR!";
  }

  char information[bytes_sent];
  bzero(information, bytes_sent);

  if (read (sd, information, bytes_sent) <= 0)
  {
    errorHandling("[ERROR] Error at reading message from server.\n");
    return "ERROR!";
  }
  string str = information;
  return str;
}

int numBytesSent(int client)
{
  int bytes_sent;
  if (read (client, &bytes_sent, sizeof(int)) <= 0) 
  {
    cout << "[server] Error at reading num bytes from client.\n";
    close(client);
    exit(1);
  }
  return bytes_sent;
}

string readingCommand_SERVER(int client)
{
  int bytes_sent = numBytesSent(client);
  char information[bytes_sent];
  bzero (information, bytes_sent);

  if (read (client, information, bytes_sent) <= 0)
  {
    cout << "[server] Error at reading command from client.\n";
    return "ERROR!";
  }

  string info = information;
  return info;
}

void sendingInfo_SERVER(int client, string information)
{
  int bytes = information.length() + 1;

  if (write (client, &bytes, sizeof(int)) <= 0)
  {
      cout << "[server] Error at writting num bytes for client.\n";
      close(client);  
      exit(1);
  }

  // trimitere comanda la server
  if (write (client, information.c_str(), bytes) <= 0)
  {
      cout << "[server] Error at writting command for client.\n";
      close(client);  
      exit(1);
  }
  else
      cout << "[server] Client has received the message.\n";
}
