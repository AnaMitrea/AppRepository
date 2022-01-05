#include <stdio.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <signal.h>
#include <iostream>
using namespace std;

#define SIZE 1024
int port = 8080;

int main ()
{
    char *ip = (char*)"127.0.0.1";
    
    int e;

    int sd, new_sd;
    struct sockaddr_in server_addr, new_addr;
    socklen_t addr_size;
    char buffer[SIZE];

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

    if(bind(sd,(struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("[-]Error in Binding");
        exit(1);
    }
    printf("[+]Binding Successfull.\n");

    if(listen(sd, 5) ==0)
    {
        printf("[+]Listening...\n");
    }
    else 
    {
        perror("[-]Error in Binding");
        exit(1);
    }
    addr_size = sizeof(new_addr);
    new_sd = accept(sd,(struct sockaddr*)&new_addr, &addr_size);


    // aici primeste numele fisierului
    FILE *fp;
    fp = fopen("discord2.deb", "ab");

    if(NULL == fp)
    {
        printf("Error opening file");
        return 1;
    }

    int bytesReceived = 0;
    char recvBuff[1024];

    while ( (bytesReceived = read(new_sd, recvBuff, 1024)) > 0 )
    {
        fflush(stdout);
        fwrite(recvBuff, 1, bytesReceived, fp);
    }

    if(bytesReceived < 0)
    {
        printf("\n Read Error \n");
    }
    printf("\nFile OK....Completed\n");

    fclose(fp);
    close(new_sd);
    close(sd);
}
