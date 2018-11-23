/*  Make the necessary includes and set up the variables.  */

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <signal.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    int server_sockfd, client_sockfd;
    int server_len, client_len;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;

/*  Remove any old socket and create an unnamed socket for the server.  */

    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);

/*  Name the socket.  */

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(9734);
    server_len = sizeof(server_address);
    bind(server_sockfd, (struct sockaddr *)&server_address, server_len);

/*  Create a connection queue and wait for clients.  */

    listen(server_sockfd, 5);

    signal(SIGCHLD, SIG_IGN);

    while(1) {
	int clientid;
	time_t courtime;
	struct tm *loctime;
	char fecha[40];
	
	courtime = time (NULL);
	loctime = localtime(&courtime); 
	//fputs(asctime(loctime),stdout);       
	strftime(fecha,40,"%A %B %d %I:%M %p.\n",loctime);
	printf("Fecha por mandar: %s", fecha);
        printf("server waiting\n");
	
/*  Accept a connection.  */
	
        client_len = sizeof(client_address);
        client_sockfd = accept(server_sockfd, 
        	(struct sockaddr *)&client_address, &client_len);
	
/*  We can now read/write to client on client_sockfd.  */
	if(fork()==0)
	{
		read(client_sockfd, &clientid, sizeof(clientid));
		printf("Se ha conectado el cliente: %d\n", clientid);
        	write(client_sockfd, &fecha, 40);
        	close(client_sockfd);
		exit(0);
	}
	else
	{
		close(client_sockfd);
	}
    }
}

