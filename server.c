//headers
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>


/*char webpage[]=
"200 OK\r\n"
"Content-Type: text/html; charset=UTF-8\r\n\r\n"

char webpage1[]=
"200 OK\r\n"
"Content-Type: text/html; charset=UTF-8\r\n\r\n"

char webpage2[]=
"200 OK\r\n"
"Content-Type: text/html; charset=UTF-8\r\n\r\n"*/

void header (int handler, int status){
	char header [] = {0};
	if (status == 0) {
		sprintf(header, "200 OK\r\n\r\n");
	} 
	else {
		sprintf(header, "404 Not Found\r\n\r\n");
	}
	send (handler, header, strlen(header), 0);
}


//main function
int main (int argc, char *argv[]){
	//socket address
	struct sockaddr_in server_addr, client_addr;	
	socklen_t sin_len = sizeof(client_addr);
	//file descriptor
	int fd_server, fd_client, SERV_TCP_PORT; 
	char buf[2048], msgrcv[500];
	int fdopen;
	int on = 1;
	const char *ptr;
	
	fd_server = socket(AF_INET, SOCK_STREAM, 0);

		if (fd_server < 0){
			perror("socket");
			exit(1);
		}
	setsockopt(fd_server, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(int));
	bzero(&server_addr, sizeof(server_addr));
	bzero(msgrcv, sizeof(msgrcv));	
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(SERV_TCP_PORT);
	
	if (bind(fd_server,(struct sockaddr *) &server_addr, sizeof(server_addr)) == -1){
		perror("bind");
		close(fd_server);
		exit(1);
	}
	//concurrent10
	if (listen(fd_server, 10) == -1){
		perror("listen");
		close(fd_server);
		exit(1);
	}
	
	printf("connection from %s, port %d\n", ptr, ntohs(client_addr.sin_port));
	
	while (1){
		fd_client = accept(fd_server, (struct sockaddr *) &client_addr, &sin_len);
		if (fd_client == -1){
			perror ("Connection failed...\n");
			continue;
		}
		printf ("GOT client connection.......\n");
		
		if (!fork()){
			/* child process */
			close (fd_server);
			memset(buf, 0, 2048);
			read (fd_client, buf, 2047);
			
			printf("%s\n", buf);
			
			if (!strncmp(buf, "GET red.html HTTP", 17)){
				
				fdopen = open ("red.html", O_RDONLY);
				sendfile(fd_client, fdopen, NULL, 350);
				close(fdopen);
			}

			else if (!strncmp(buf, "GET blue.html HTTP", 18)){

                                fdopen = open ("blue.html", O_RDONLY);
                                sendfile(fd_client, fdopen, NULL, 350);
                                close(fdopen);
                        }

			else if (!strncmp(buf, "GET green.html HTTP", 18)){

                                fdopen = open ("green.html", O_RDONLY);
                                sendfile(fd_client, fdopen, NULL, 350);
                                close(fdopen);
                        }

			else if (!strncmp(buf, "PUT test.txt", 12)){
				
				FILE* file_ptr = fopen("test.txt", "w");
    				fclose(file_ptr);
			}
			
			else 
			write (fd_client, header, sizeof(header) -1 );
			
			close (fd_client);
			printf("closing.....\n");
			exit(0);
		
		}
		/* Parent Process */
		close(fd_client);
		
	}
	
	return 0;
}
