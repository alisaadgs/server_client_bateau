#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 4443

int main(){

	int sd;
	int s; // accept
	struct sockaddr_in serveur,client; 
	char mes[256];
	char x[10], y[10];
	
	char force[]={'0','1','2','3','4'};
	char direction[]={'N','S','E','O'};
	
	char vent[]={'0','N'};
	//printf("%s\n",vent);


	FILE *fptr;     
	fptr = fopen("bateau.txt","w");
	if(fptr == NULL)
	{
		printf("Error!");   
		exit(1);             
	}
	fclose(fptr);
	
	
	
	int sockfd, ret;
	 struct sockaddr_in serverAddr;

	int newSocket;
	struct sockaddr_in newAddr;
	
	socklen_t addr_size;
	
	pid_t childpid;
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		printf("Erreur de connection.\n");
		exit(1);
	}
	printf("socket serveur est créé.\n");
	
	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("Erreur de binding.\n");
		exit(1);
	}
	printf("Bind a port %d\n", 4444);

	if(listen(sockfd, 10) == 0){
		printf("en ecoute....\n");
	}else{
		printf("Erreur de binding.\n");
	}


	while(1){      //boucle infini sur fonction accept() pour accepter multi-client
		s = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
		if(newSocket < 0){
			exit(1);
		}
		printf("Connection acceptee de %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
		
	
	
		if((childpid = fork()) == 0){
			close(sockfd);

  			while(1){    //le serveur est en ecoute permanant pour recevoir les parametres du vent (force et direction)
			
			printf("\n donne moi la force et la direction du vent: ");
			scanf("%s",mes);
			//sen[]='\0';
			sd=send(s,mes,strlen(mes),0);			
		
		}
	  }
	
	

	}

	close(newSocket);


	return 0;
}
