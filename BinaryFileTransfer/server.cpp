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
#include <iostream>
using namespace std;

void sendFile_to_CLIENT(int client, string fname)
{
    FILE* fp = fopen(fname.c_str(),"rb");
    if(fp == NULL)
    {
        perror("[-]Error in reading file.");
        exit(1);
    }

    while(1)
    {
        unsigned char buff[512];
        bzero(buff,512);

        int nread = fread(buff,1,512,fp);

        if( nread > 0 )
        {
            write(client, buff, nread);
        }

        if( nread < 512 )
        {
            if( feof(fp) )
            {
                printf("End of file\n");
                break;
            }
            if(ferror(fp))
            {
                printf("error reading");
                break;
            }        
        }
    }
}

int existing_file_check(string file_name)
{
  if(access(file_name.c_str(), F_OK) == 0) // file exists
    return 1;
  else
    return -1;
}

int main ()
{
    char *ip = (char*)"127.0.0.2";
    int port = 8080;
    int e;

    int sd, client;
    struct sockaddr_in server_addr, new_addr;
    socklen_t addr_size;
    char buffer[512];

    sd = socket(AF_INET, SOCK_STREAM, 0);
    if(sd<0)
    {
        perror("[-]Error in socket");
        exit(1);
    }
    printf("[+]Server socket created. \n");

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = port;
    server_addr.sin_addr.s_addr = inet_addr(ip);

    int enable = 1;
    if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
    {
        cout << "setsockopt(SO_REUSEADDR) failed.\n";
        return 0;
    }

    if(bind(sd,(struct sockaddr*)&server_addr, sizeof(server_addr))<0)
    {
        perror("[-]Error in Binding");
        exit(1);
    }
    printf("[+]Binding Successfull.\n");

    if(listen(sd, 5) == -1)
    {
        perror("[-]Error in listen");
        exit(1);
    }

    printf("[+]Listening...\n");

    addr_size = sizeof(new_addr);
    client = accept(sd,(struct sockaddr*)&new_addr, &addr_size);






    if(existing_file_check("apps/discord.exe") == -1)
    {
        cout << "Error: File doesn't exist!" << endl;
        close(client);
        close(sd);
        return 0;
    }
    
    sendFile_to_CLIENT(client,"apps/discord.deb");


    
    close(client);
    close(sd);
}
