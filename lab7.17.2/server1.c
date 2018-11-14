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

int sum(int val[], int size)
{
    int i;
    int tmp = 0;
    for(i = 0;i<size;i++)
    {
	tmp = tmp + val[i];
    }
    return tmp;
}

int res(int val[], int size)
{
    int i;
    int tmp = val[0];
    for(i = 1;i<size;i++)
    {
	tmp = tmp - val[i];
    }
    return tmp;
}

int mul(int val[], int size)
{
    int i;
    int tmp = 1;
    for(i = 0;i<size;i++)
    {
	tmp = tmp*val[i];
    }
    return tmp;
}

int main()
{
    int server_sockfd, client_sockfd;
    int server_len, client_len;
    struct sockaddr_un server_address;
    struct sockaddr_un client_address;

/*  Remove any old socket and create an unnamed socket for the server.  */

    unlink("server_socket");
    server_sockfd = socket(AF_UNIX, SOCK_STREAM, 0);

/*  Name the socket.  */

    server_address.sun_family = AF_UNIX;
    strcpy(server_address.sun_path, "server_socket");
    server_len = sizeof(server_address);
    bind(server_sockfd, (struct sockaddr *)&server_address, server_len);

/*  Create a connection queue and wait for clients.  */

    listen(server_sockfd, 5);
    while(1) {
	int i;
	long ans = 0;
	item item;

        printf("server waiting\n");

/*  Accept a connection.  */

        client_len = sizeof(client_address);
        client_sockfd = accept(server_sockfd, 
            (struct sockaddr *)&client_address, &client_len);

/*  We can now read/write to client on client_sockfd.  */

        read(client_sockfd, &item, sizeof(item));

	if(item.op == 's')
	{
		ans = sum(item.val,item.size);
	}
	else if(item.op == 'r')
	{
		ans = res(item.val,item.size);
	}
	else if(item.op == 'm')
	{
		ans = mul(item.val,item.size);
	}
	else
	{
		ans = -99999;
	}

        printf("Server: answer = %ld\n",ans);
        write(client_sockfd, &ans, sizeof(long));
        close(client_sockfd);
    }
}

