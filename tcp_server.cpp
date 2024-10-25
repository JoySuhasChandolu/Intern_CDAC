#include<iostream>
#include<stdio.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<string.h>

int main(int argc,char *argv[]){
	
	
	char buffer[512];
	

    struct sockaddr_in servaddr,cliaddr;

	if(argc != 2){
		printf("USAGE : server <portno>");
		exit(1);
	}
	
	int sd = socket(PF_INET,SOCK_STREAM,0);

	if(sd < 0){
		perror("socket error");
	}

	int PORT = htons(atoi(argv[1]));

	servaddr.sin_family = AF_INET;
	servaddr.sin_port = PORT;
	servaddr.sin_addr.s_addr = INADDR_ANY;

	memset(servaddr.sin_zero,'\0',sizeof servaddr.sin_zero);

	if(bind(sd,(struct sockaddr *)&servaddr,sizeof servaddr)<0){
		perror("bind error");
		exit(1);
	}

	//to remove "address already in use" error
	int yes = 1;

	if (setsockopt(sd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof yes) == -1){
		perror("setsockpot error");
		exit(1);
	}

	for(;;){
		
		listen(sd,10);

		int cliaddrlength = sizeof(cliaddr);
	
		int new_fd = accept(sd,(struct sockaddr *)&cliaddr,(socklen_t *)&cliaddrlength);

		int bytes_received = recv(new_fd,(char *)buffer,sizeof(buffer),0);

		int bytes_sent = send(new_fd,(char *)buffer,bytes_received,0); 
	}

	close(sd);

	return 0;
}