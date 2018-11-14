/*  Make the necessary includes and set up the variables.  */

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct{
	char op;
	int size;
	int val[10];
} item;

int main(int argc, char *argv[])
{
    int sockfd;
    int len,i;
    struct sockaddr_un address;
    int result;
    long ans = 0;
    item item;

/*  Create a socket for the client.  */

    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);

/*  Name the socket, as agreed with the server.  */

    address.sun_family = AF_UNIX;
    strcpy(address.sun_path, "server_socket");
    len = sizeof(address);

/*  Now connect our socket to the server's socket.  */

    result = connect(sockfd, (struct sockaddr *)&address, len);

    if(result == -1) {
        perror("Client: oops: client1");
        exit(1);
    }

/*  We can now read/write via sockfd.  */

    item.op = *argv[1];
    item.size = argc-2;
    for(i=2;i<argc;i++)
    {
	item.val[i-2] = atoi(argv[i]);
    }

    write(sockfd, &item, sizeof(item));
    read(sockfd, &item, sizeof(long));
    if(ans=-99999)
    {
	printf("Unsupported operation\n");
    }
    else
    {
	printf("Client: answer from server = %ld\n", ans);
    }
    close(sockfd);
    exit(0);
}
