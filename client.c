#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>

#define MAXSIZE 2048
#define PORT 8888

int main(int argc, char **argv)
{
    char buffer[MAXSIZE];
    int port , socket_fd;
    int size;
    struct sockaddr_in addr;    

    port = PORT;
    if(argc > 1)
    {
        port = atoi(argv[1]);  
    }
    
    /* creat a socket */
    socket_fd = socket(AF_INET,SOCK_STREAM,0);
    
    printf("%d\n",socket_fd);

    /* init the address of server */ 
    memset(&addr,0,sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  
    /* client request connection to server*/
    if(connect(socket_fd,(struct sockaddr *)&addr,sizeof(addr)) < 0)
    {
        perror("connect");
        exit(1);
    }


    /* send request msg to server*/
    char *req = "Hi";    
    if(send(socket_fd,req,strlen(req),0) < 0)
    {
        printf("send msg error: %s(errno: %d)\n",strerror(errno),errno);
        exit(0);
    }
    /* receive response msg from server */ 
    if((size = recv(socket_fd,buffer,MAXSIZE,0)) == -1)
    {
        size = 0;
        perror("recv error");
        close(socket_fd); 
        exit(1);
    }   
     
    
    buffer[size] = '\0';
    printf("recv msg from server: %s\n",buffer);
    close(socket_fd);
    return 0;
}

