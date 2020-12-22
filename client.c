#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdbool.h>
void error(const char *msg)
{
    perror(msg);
    printf("closing connection with the server\n");
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n,c=0,temp,subject_no,choice,count_login,count_attempt;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    float percentage,marks,new_marks;
    char buffer[256],name[20],password[20];
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);

    // create stream socket

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    //build server address structure

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);

    //connect to server

    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");
    else{printf("connected to server successfully\n");}

    //Enter username and password

    bzero(buffer,256);
    bzero(name,20);
    bzero(password,20);
    count_login=1;
    do
    {
        printf("\nEnter Username : ");
        scanf("%s",name);
        n=write(sockfd,name,20);
        if (n < 0)
         error("ERROR writing to socket");
        printf("Enter password : ");
        scanf("%s",password);
        n=write(sockfd,password,20);
        if (n < 0)
         error("ERROR writing to socket");
        n=read(sockfd,&c,sizeof(int));
        if (n < 0)
         error("ERROR reading from socket");
        if(c==0)
            printf("\nWrong Username or Password:Attempts remaining=%d\n",5-count_login);
        count_login++;
        if(count_login==6)
            error("Too many wrong attempts");
    }while(c==0);
    return 0;
}
