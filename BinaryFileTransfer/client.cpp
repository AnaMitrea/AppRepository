#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <string>
#include <sys/wait.h>
#include <netdb.h>
#include <signal.h>
#include <iostream>
using namespace std;

int port = 8080;

int main()
{
    char *ip = (char*)"127.0.0.1";
    int e;

    int sd;
    struct sockaddr_in server_addr;

    FILE *fp;


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

    e = connect(sd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if(e == -1)
    {
        perror("[-]Error in Connecting");
        exit(1);
    }

    printf("[+]Connected to server.\n");

    cout << "File name: ";
    string msg;
    msg.clear();
    fflush(stdout);
    getline(cin,msg);

    char *filename = (char*)msg.c_str();
    cout << "\nfilename= \"" << filename << "\"\n";

    fp = fopen(filename, "rb");
    if(fp == NULL)
    {
        perror("[-]Error in reading file.");
        exit(1);
    }

    /* Read data from file and send it */
    while(1)
    {
        unsigned char buff[1024]={0};
        int nread = fread(buff,1,1024,fp);

        if( nread > 0 )
        {
            write( sd , buff , nread );
        }

        if( nread < 1024 )
        {
            if(feof(fp)) // EOF
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

    
    close(sd);
    fclose(fp);

    return 0;

}
