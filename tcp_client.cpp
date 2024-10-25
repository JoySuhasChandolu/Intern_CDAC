
#include<stdio.h>
#include<sys/types.h>
#include<iostream>
#include<sys/socket.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>

int main(int argc,char ** argv){
	
	//int sd,i;
	
	
	struct sockaddr_in servaddr,cliaddr;

	if(argc != 4){
		printf("Client <port> <server> <message>");
		exit(1);
	}
	
	int sd = socket(PF_INET,SOCK_STREAM,0);

	if(sd < 0){
		perror("socket error");
	}

	int PORT = htons(atoi(argv[1]));

	servaddr.sin_family = AF_INET;
	servaddr.sin_port = PORT;
	servaddr.sin_addr.s_addr = inet_addr(argv[2]);

	int servaddrlen = sizeof(servaddr);


	connect(sd,(struct sockaddr *)&servaddr,servaddrlen);

	for(;;){

		char *buffer = new char[50];

		std::cin >> buffer;

		int  bytes_sent = send(sd,buffer,strlen(buffer),0);

		int bytes_received = recv(sd,buffer,bytes_sent,0);

		std::cout<<buffer<<"\n";

		memset(buffer,'\0',strlen(buffer));
	}

	close(sd);
}