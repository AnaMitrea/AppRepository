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


int receiveFile_from_SERVER(int sd, string fname)
{
    FILE *fileptr = fopen(fname.c_str(), "wb"); // binary

    if(fileptr == NULL)
    {
        printf("Error opening file");
        return -1;
    }

    int bytesReceived = 0;
    int bytesRec = 0;
    char recvBuff[512];
    bzero(recvBuff,512);

    while(1)
    {
        if((bytesRec = read(sd, recvBuff, 512)) < 0)
        {
            perror("read error in while");
            exit(1);
        }

        if(bytesRec == 0)
            break;
            
        fwrite(recvBuff, 1, bytesRec, fileptr);
        bzero(recvBuff,512);

        if( bytesRec < 512 )
            break;
    }

    if(bytesReceived < 0)
    {
        printf("\nRead Error \n");
        fclose(fileptr);
        return -1;
    }

    fclose(fileptr);
    return 1;
}


int main()
{
    char *ip = (char*)"127.0.0.2";
    int port = 8080;
    int e;

    int sd;
    struct sockaddr_in server_addr;

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

    if(connect(sd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("[-]Error in Connecting");
        exit(1);
    }

    printf("[+]Connected to server.\n");



    int ok = receiveFile_from_SERVER(sd,"discord2.deb");
    cout << "ok= " << ok << endl;


    
    close(sd);
    
    return 0;

}
