#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#define PORT 5620

int main(int argc, char const *argv[]){ 

    int server_fd, new_socket, valread; 

    struct sockaddr_in address; 

    int opt = 1; 

    int addrlen = sizeof(address); 

    char buffer[1024] = {0}; 

	const char *servermessage="Server received message: ";
       
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0){ 

        perror("Socket failed. "); 

        exit(EXIT_FAILURE); 
    } 
       

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,&opt, sizeof(opt))){ 

        perror("Setsockopt error. "); 

        exit(EXIT_FAILURE); 
    } 


    address.sin_family = AF_INET; 

    address.sin_addr.s_addr = INADDR_ANY; 

    address.sin_port = htons( PORT ); 
       
    if (bind(server_fd, (struct sockaddr *)&address,sizeof(address))<0){ 

        perror("Bind failed. "); 
        exit(EXIT_FAILURE); 
    } 


    if (listen(server_fd, 3) < 0){ 

        perror("Listen error. "); 

        exit(EXIT_FAILURE); 
    } 


    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,(socklen_t*)&addrlen))<0){
 
        perror("Accept error. "); 

        exit(EXIT_FAILURE); 
    } 


	printf("Host connected.\n");

	while(1){

		for(int i=0;i<1024;++i)buffer[i]=0;

		valread = read( new_socket , buffer, 1024); 

		printf("Message received: %s",buffer ); 

		send(new_socket , servermessage , strlen(servermessage) , 0 ); 

		send(new_socket , buffer , valread , 0 ); 
	}


    return 0; 
} 



