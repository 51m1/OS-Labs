/*  Make the necessary includes and set up the variables.  */

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[])
{
    int sockfd;
    int len,i;
    struct sockaddr_in address;
    int result;
    int sid = atoi(argv[1]);
    char fecha[40];

/*  Create a socket for the client.  */

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

/*  Name the socket, as agreed with the server.  */

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = htons(9734);
    len = sizeof(address);

/*  Now connect our socket to the server's socket.  */

    result = connect(sockfd, (struct sockaddr *)&address, len);

    if(result == -1) {
        perror("oops: client3");
        exit(1);
    }

/*  We can now read/write via sockfd.  */
	
    write(sockfd, &sid, sizeof(sid));

    for(i=0; i<10;i++)
    {
    	read(sockfd, &fecha, 40);
    	printf("Date from server = %s\n", fecha);
	sleep(3);
    }
    close(sockfd);
    exit(0);
}
