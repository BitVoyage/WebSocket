#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<errno.h>
#include<netinet/in.h>

#define PROCESS_PORT 8888
#define MAXSIZE 2048
#define FILE_NAME_MAXSIZE 512

int main(int argc , char ** argv)
{
    int socket_fd , client_fd;
    int PORT;
    int size,n;
    char buffer[MAXSIZE];
    char *resp = "Hi!";
    struct sockaddr_in server_addr , client_addr;

    /* creat a socket */
    if((socket_fd = socket(PF_INET,SOCK_STREAM,0))==-1)
    {
        printf("create socket error: %s(errno: %d)\n",strerror(errno),errno);
        exit(0);
    }

    PORT = PROCESS_PORT;
    if(argc > 1)
    {
        PORT = atoi(argv[1]);
    }

    /* init the address and port */
    memset(&server_addr , 0 , sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    /* bind address to the socket we have created above */
    if(bind(socket_fd,(struct sockaddr*)&server_addr,sizeof(server_addr)) == -1)
    {
        printf("bind socket error: %s(errno: %d)\n",strerror(errno),errno);
        exit(0);
    }

    /* listen the port */
    if(listen(socket_fd,100) == -1)
    {
        printf("listen socket error: %s(errno: %d)\n",strerror(errno),errno);
        exit(0);
    }

    printf("waiting for client's request\n");

    /* accept the connection of client and send msg to client or receive msg from client */
    while(1)
    {
        if((client_fd = accept(socket_fd,(struct sockaddr*)&client_addr,&size)) == -1)
        {
            printf("accept socket error: %s(errno : %d)\n",strerror(errno),errno);
            continue;
        }

 //       printf ("server: accepting a client from %s port %d\n",client_addr.sin_addr,client_addr.sin_port);

        /* Prepare the file sended */
        bzero(buffer,sizeof(buffer));
        int length = recv(client_fd, buffer,MAXSIZE,0);
        if(length < 0){
            printf("Server Receive Data Failed");
            break;
        }

        char file_name[FILE_NAME_MAXSIZE+1];
        bzero(file_name,sizeof(file_name));
        strncpy(file_name, buffer, strlen(buffer) > FILE_NAME_MAXSIZE ? FILE_NAME_MAXSIZE : strlen(buffer));

        char path[]="./ServerFile/";
        strcat(path, file_name);
        FILE *fp = fopen(path, "r");
        if(fp == NULL)
            printf("File:\t%s Not Found!\n", file_name);
        else{
            bzero(buffer, MAXSIZE);
            int file_block_length = 0;
            while((file_block_length = fread(buffer, sizeof(char), MAXSIZE, fp)) > 0){
                printf("file_block_length = %d\n", file_block_length);

                /* sending file */
                if(send(client_fd, buffer, file_block_length,0) < 0){
                    printf("Send File:\t%s Failed!\n", file_name);
                    break;
                    }
                bzero(buffer,sizeof(buffer));;
                }
            fclose(fp);
            printf("File:\t%s Transfer Finisheda!\n", file_name);
        }

        close(client_fd);
    }
    close(socket_fd);

    return 0;
}
