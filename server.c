#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>

#define   MINHA_PORTA   2000
#define   BACKLOG       5

int main(int argc, char const *argv[])
{
	int mySocket, newSocket, size;

	struct sockaddr_in local;
	struct sockaddr_in remote;

	if ((mySocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
	{
		perror("socket");
		exit(1);
	}

	local.sin_family = AF_INET;
	local.sin_port = htons(MINHA_PORTA);
	local.sin_addr.s_addr = INADDR_ANY;

	bzero(&(local.sin_zero), 8);

	if (bind(mySocket, (struct sockaddr *)&local, sizeof(struct sockaddr))== -1) 
	{
		perror("bind");
		exit(1);
	}

	if (listen(mySocket, BACKLOG) < 0) 
	{
		perror("listen");
		exit(1);
	}

	while(1){
		size = sizeof(struct sockaddr_in);

		if ((newSocket = accept(mySocket, (struct sockaddr *)&remote,&size)) < 0){
			perror("accept");
			continue;
		}

		if (!fork()) {
			if (send(newSocket, "Seja bem vindo!\n", 16, 0) == -1)
			{
				perror("send");
				close(newSocket);
				exit(0);
			}
		}

		close(newSocket);
		while(waitpid(-1,NULL,WNOHANG) > 0);
	}

	return 0;
}