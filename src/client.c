/* 
    ********************************************************************
    Odsek:          Elektrotehnika i racunarstvo
    Departman:      Racunarstvo i automatika
    Katedra:        Racunarska tehnika i racunarske komunikacije (RT-RK)
    Predmet:        Osnovi Racunarskih Mreza 1
    Godina studija: Treca (III)
    Skolska godina: 2021/22
    Semestar:       Zimski (V)
    
    Ime fajla:      client.c
    Opis:           TCP/IP klijent
    
    Platforma:      Raspberry Pi 2 - Model B
    OS:             Raspbian
    ********************************************************************
*/

#include<stdio.h>      //printf
#include<string.h>     //strlen
#include<sys/socket.h> //socket
#include<arpa/inet.h>  //inet_addr
#include <fcntl.h>     //for open
#include <unistd.h>    //for close

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT   27016

int main(int argc , char *argv[])
{
    int sock;
    struct sockaddr_in server;
    char client_message[DEFAULT_BUFLEN];
    char server_message[DEFAULT_BUFLEN];
    
    char server_command[DEFAULT_BUFLEN];

    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(DEFAULT_PORT);

    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }

    puts("Connected\n");
    
    while(1){
    	if(strcmp(server_command, "LOGIN") == 0)
    	{
    		printf("\tLOGIN | REGISTER | BACK\n");
    		printf("\tEnter the command: "); 
    	}
    	else if(strcmp(server_command, "LOG_1") == 0)
    	{
    		printf("\tLOGIN: Enter username or membership card number: "); 
    	}
    	else if(strcmp(server_command, "REG_1") == 0)
    	{
    		printf("\tREGISTRATION: Enter username: "); 
    	}
    	else if(strcmp(server_command, "LOG_2") == 0)
    	{
    		printf("\tLOGIN: Enter password: "); 
    	}
    	else if(strcmp(server_command, "REG_2") == 0)
    	{
    		printf("\tREGISTRATION: Enter Password: "); 
    	}
    	else if(strcmp(server_command, "SEARCH") == 0)
    	{
    		printf("\tSEARCHALL | SEARCH | BACK\n");
    		printf("\tEnter the command: "); 
    	}
    	else if(strcmp(server_command, "SEARCHID") == 0)
    	{
    		printf("\tID: ");
    	}
    	else if(strcmp(server_command, "SEARCHAUTHOR") == 0)
    	{
    		printf("\tAuthor: ");
    	}
    	else if(strcmp(server_command, "SEARCHNAME") == 0)
    	{
    		printf("\tName: ");
    	}
    	else if(strcmp(server_command, "SEARCHYEAR") == 0)
    	{
    		printf("\tYear: ");
    	}
    	else if(strcmp(server_command, "RESERVE") == 0)
    	{
    		printf("\tID: ");
    	}
    	else
    	{
    		printf("LOGIN | LOGOUT | SEARCH | CHECKSTATUS | RESERVE\n");
    		printf("Enter the command: "); 	
	}
	
	
	fgets(client_message, DEFAULT_BUFLEN, stdin);
	
    	//Send some data
    	if(send(sock , client_message , strlen(client_message), 0) < 0)
    	{
	   	puts("Send failed");
		return 1;
    	}	 
	
    
    	//Receive response from server
    	if(recv(sock, server_message, sizeof(server_message), 0) < 0){
		printf("Error while receiving server's msg\n");
		return -1;
    	}
    	if(recv(sock, server_command, sizeof(server_command), 0) < 0){
		printf("Error while receiving server's cmd\n");
		return -1;
    	}
 	
	if(server_message[0] != '\0') printf(" %s\n", server_message);	

    }

    close(sock);

    return 0;
}

