#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>

#define MINHA_PORTA   2000
#define MAX_SIZE 100

int main(int argc, char const *argv[])
{
	int mySocket, numBytes;
	char buf[MAX_SIZE];

	clock_t timeout;
	
	struct sockaddr_in client;

	if (argc != 2)
	{
		perror("argc");
		exit(1);
	}

	if (argv[1] == NULL)
	{
		perror("Endereco ip");
		exit(1);
	}

	if ((mySocket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("Socket");
		exit(1);
	}

	client.sin_family = AF_INET;
	client.sin_port = htons(MINHA_PORTA);
	client.sin_addr.s_addr = inet_addr(argv[1]);
	bzero(&(client.sin_zero), 8);

	timeout = clock();

	if ((connect(mySocket, (struct sockaddr *)&client, sizeof(client))) == -1)
	{
		perror("Conector");
		exit(1);
	}

	if ((numBytes = recv(mySocket, buf, MAX_SIZE, 0)) == -1)
	{
		perror("recv");
		exit(1);
	}

	timeout = clock() - timeout;
	float t = ((float)timeout) / CLOCKS_PER_SEC;
	printf("Tempo para conectar %f segundos, CLOCKS_PER_SEC: %f, timeout: %d \n", t, CLOCKS_PER_SEC, timeout);

	buf[numBytes] = '\0';
	printf("Recebido: %s\n",buf);

	close(mySocket);

	return 0;
}