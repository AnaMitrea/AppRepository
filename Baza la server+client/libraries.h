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
#include <unistd.h>

// CLIENT FUNCTIONS
void errorHandling(string errmsg);
void printInstructions();
string readingInfo_CLIENT(int sd);
void sendingCommand_CLIENT(int sd, int bytes, string command);
int existing_file_check(string fname);
int receiveFile_from_SERVER(int sd, string fname);
void sendFile_to_SERVER(int sd, string fname);

// SERVER FUNCTIONS
#define PORT 2024
void sighandler();
string readingCommand_SERVER(int client);
void sendingInfo_SERVER(int sd);
int numBytesSent(int client);
void sendFile_to_CLIENT(int client, string fname);
int receiveFile_from_CLIENT(int client, string fname);


int receiveFile_from_CLIENT(int client, string fname) // file saves as apps/nr.extension
{
  FILE *fileptr = fopen(fname.c_str(), "wb"); // binary
  if(fileptr == NULL)
  {
    cout << "[server] Error - File couldn't be opened/created!";
    return -1;
  }

  int bytesRec = 0;
  char information[512];
  bzero(information, 512);

  while(1)
  {
    if((bytesRec = read(client, information, 512)) < 0)
    {
      cout << "[server] Reading error\n";
      fclose(fileptr);
      exit(1);
    }
    else
    if(bytesRec == 0)
    {
      break;
    }
  
    fwrite(information,1,bytesRec,fileptr);
    bzero(information,512);

    if(bytesRec < 512) // the last bytes of the executable
    {
      break;
    }
  }

  if(bytesRec < 0)
  {
    cout << "[client] Reading error\n";
    fclose(fileptr);
    return -1;
  }

  fclose(fileptr);
  return 1;
}

void sendFile_to_SERVER(int sd, string fname) //file name as nr.extension
{
  FILE* ptr = fopen(fname.c_str(),"rb");
  if(ptr == NULL)
  {
    cout << "[client] Reading file error.\n";
    exit(1);
  }

  unsigned char buff[512];
  while(1)
  {
    bzero(buff,512);

    int bytesSent = fread(buff,1,512,ptr);
    if(bytesSent > 0)
    {
      write(sd,buff,bytesSent);
    }

    if(bytesSent < 512)
    {
      if(feof(ptr))
      {
        cout << "[client] File was sent succesfully.\n";
        break;
      }
      if(ferror(ptr))
      {
        cout << "[client] Reading file error.\n";
        break;
      }        
    }
  }
}

int receiveFile_from_SERVER(int sd, string fname)
{
  FILE *fileptr = fopen(fname.c_str(), "wb"); // binary
  if(fileptr == NULL)
  {
    cout << "[client] Error - File couldn't be opened/created!";
    return -1;
  }

  int bytesRec = 0;
  char information[512];
  bzero(information, 512);

  while(1)
  {
    if((bytesRec = read(sd, information, 512)) < 0)
    {
      cout << "[client] Reading error\n";
      fclose(fileptr);
      exit(1);
    }
    else
    if(bytesRec == 0)
    {
      break;
    }
  
    fwrite(information,1,bytesRec,fileptr);
    bzero(information,512);

    if(bytesRec < 512) // the last bytes of the executable
    {
      break;
    }
  }

  if(bytesRec < 0)
  {
    cout << "[client] Reading error\n";
    fclose(fileptr);
    return -1;
  }

  fclose(fileptr);
  return 1;
}

void sendFile_to_CLIENT(int client, string fname)
{
  FILE* ptr = fopen(fname.c_str(),"rb");
  if(ptr == NULL)
  {
    cout << "- Reading file error.\n";
    exit(1);
  }

  unsigned char buff[512];
  while(1)
  {
    bzero(buff,512);

    int bytesSent = fread(buff,1,512,ptr);
    if(bytesSent > 0)
    {
      write(client, buff, bytesSent);
    }

    if(bytesSent < 512)
    {
      if(feof(ptr))
      {
        cout << "- File pointer is at EOF.\n";
        break;
      }
      if(ferror(ptr))
      {
        cout << "- Reading file error.\n";
        break;
      }        
    }
  }
}

int existing_file_check(string fname)
{
  if(access(fname.c_str(), F_OK) == 0) // file exists
    return 1;
  else
    return -1;
}

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
    cout << "[server] Error at reading num bytes from client. Client has been disconnected.\n";
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
    cout << "[server] Error at reading command from client. Client disconnected.\n";
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
    cout << "[server] Error at writting num bytes for client. Client disconnected.\n";
    close(client);  
    exit(1);
  }

  if (write (client, information.c_str(), bytes) <= 0)
  {
    cout << "[server] Error at writting command for client. Client disconnected.\n";
    close(client);  
    exit(1);
  }
  else
    cout << "[server] Client has received the message.\n";
}
