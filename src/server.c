/* 
    ********************************************************************
    Odsek:          Elektrotehnika i racunarstvo
    Departman:      Racunarstvo i automatika
    Katedra:        Racunarska tehnika i racunarske komunikacije (RT-RK)
    Predmet:        Osnovi Racunarskih Mreza 1
    Godina studija: Treca (III)
    Skolska godina: 2021/22
    Semestar:       Zimski (V)
    
    Ime fajla:      server.c
    Opis:           TCP/IP server
    
    Platforma:      Raspberry Pi 2 - Model B
    OS:             Raspbian
    ********************************************************************
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include<pthread.h>   //thread

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT   27016

#define MAX_USERS 256
#define MAX_ARTWORKS 10

void *ThreadJoin(void *clientSocket);

struct user {
    char username[DEFAULT_BUFLEN];
    char cardNumber[DEFAULT_BUFLEN];
    char password[DEFAULT_BUFLEN];
};

struct artwork {
    char id[DEFAULT_BUFLEN];
    char author[DEFAULT_BUFLEN];
    char name[DEFAULT_BUFLEN];
    char year[DEFAULT_BUFLEN];
    char reserved[DEFAULT_BUFLEN];
};

int numberOfUsers = 0;
int numberOfArtworks = 0;
int numberOfSearched = 0;

struct user users[MAX_USERS];
struct artwork artworks[MAX_ARTWORKS];
struct artwork searchedArtworks[MAX_ARTWORKS];

void LoadUsers();
void WriteUsers();

void LoadArtworks();
void WriteArtworks();

int main(int argc , char *argv[])
{
    int socketDesc , clientSock , c;
    struct sockaddr_in server , client;
     
    LoadUsers();	// loading users from file (previous sessions)
    LoadArtworks();	// loading artworks from file
     
    //Create socket
    socketDesc = socket(AF_INET , SOCK_STREAM , 0);
    if (socketDesc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(DEFAULT_PORT);

    //Bind
    if(bind(socketDesc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");

    //Listen
    listen(socketDesc , 3);

    //Accept and incoming connection
    puts("Waiting for incoming connections...\n");
    c = sizeof(struct sockaddr_in);

    while(1){
	    //accept connection from an incoming client
	    clientSock = accept(socketDesc, (struct sockaddr *)&client, (socklen_t*)&c);
	    if (clientSock < 0)
	    {
		perror("accept failed");
		return 1;
	    }
	    
	    printf("Client connected at IP: %s and port: %i\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
	    
	    //creating separate threads for each user
	    pthread_t t;
	    int *pclient = malloc(sizeof(int));
	    *pclient = clientSock;
	    pthread_create(&t, NULL, ThreadJoin, pclient);	   	   
    }
    
    close(clientSock);
    close(socketDesc);
    return 0;
}

void *ThreadJoin(void *clientSocket)
{
    int readSize;
    char clientMessage[DEFAULT_BUFLEN];
    char serverMessage[DEFAULT_BUFLEN];
    char serverCommand[DEFAULT_BUFLEN];	// controls the further behavior of the client side
    int sock = *((int*)clientSocket);	
    free(clientSocket);  
    
    int loggedIn = 0;
    int loginRegister = 0; 			// whether we are logging in(1) or registering(2)
    char usernameOrCardNumber[DEFAULT_BUFLEN];	// uneti string koji moze biti ili jedno ili drugo
    char userPassword[DEFAULT_BUFLEN];
    char userID[DEFAULT_BUFLEN];
    
    char id[DEFAULT_BUFLEN] = " ", author[DEFAULT_BUFLEN] = " ", name[DEFAULT_BUFLEN] = " ", year[DEFAULT_BUFLEN] = " ";
    memset(id, 0, sizeof(serverMessage));
    memset(author, 0, sizeof(serverMessage));
    memset(name, 0, sizeof(serverMessage));
    memset(year, 0, sizeof(serverMessage));
    
    while((readSize = recv(sock , clientMessage , DEFAULT_BUFLEN , 0)) > 0)
    {   	
   	//Receive a command from client
   	
    	memset(serverMessage, 0, sizeof(serverMessage)); // delete the previous serverMessage
    
    	if(strcmp(clientMessage, "LOGIN\n") == 0 && strcmp(serverCommand, "LOGIN") == 0)
    	{
    		loginRegister = 1;
    		strcpy(serverCommand, "LOG_1");	//forward to the first part of the login form
    	}
    	else if(strcmp(clientMessage, "LOGIN\n") == 0)
    	{
    		if(loggedIn == 1) strcpy(serverMessage, "You are already logged in!");
    		else strcpy(serverCommand, "LOGIN");				
    	}
    	else if(strcmp(serverCommand, "SEARCH") == 0 && strcmp(clientMessage, "SEARCH\n") == 0)
    	{
    		strcpy(serverCommand, "SEARCHID");
    	}
    	else if(strcmp(clientMessage, "SEARCH\n") == 0)
    	{
    		if(loggedIn == 0) strcpy(serverMessage, "You are not logged in!");
    		else strcpy(serverCommand, "SEARCH");
    	}
    	else if(strcmp(clientMessage, "SEARCHALL\n") == 0 && strcmp(serverCommand, "SEARCH") == 0)
    	{
    		if(loggedIn == 0) strcpy(serverMessage, "You are not logged in");
    		else
    		{
    			//saving searched artworks for printing on client side
	    		char *x = (char*)malloc(DEFAULT_BUFLEN*numberOfArtworks*sizeof (char));
	    		strcat(x, "\nLLIST OF ARTWORKS: \n");
	    		for(int i = 0; i < numberOfArtworks; i++) 
	    		{
	    			strcat(x, "\tID:");
				strcat(x, artworks[i].id);
				strcat(x, " AUTHOR:");
				strcat(x, artworks[i].author);
				strcat(x, " NAME:");
				strcat(x, artworks[i].name);
				strcat(x, " YEAR:");
				strcat(x, artworks[i].year);
				strcat(x, "\n");
	    		}
	    		strcpy(serverMessage, x);
	    		
	    		//adding to the list of searched artworks
	    		numberOfSearched = numberOfArtworks;
	    		for(int i = 0; i < numberOfArtworks; i++)
	    		{
	    			strcpy(searchedArtworks[i].id, artworks[i].id);
	    			strcpy(searchedArtworks[i].author, artworks[i].author);
	    			strcpy(searchedArtworks[i].name, artworks[i].name);
	    			strcpy(searchedArtworks[i].year, artworks[i].year);
	    			strcpy(searchedArtworks[i].reserved, artworks[i].reserved);
	    		}
	    		
	    		memset(serverCommand, 0, sizeof(serverCommand));
	    	}
    	}
    	else if(strcmp(serverCommand, "SEARCHID") == 0)
    	{
    		clientMessage[readSize-1] = '\0';
		strcpy(id, clientMessage); 
		strcpy(serverCommand, "SEARCHAUTHOR");
	}
	else if(strcmp(serverCommand, "SEARCHAUTHOR") == 0)
	{
		clientMessage[readSize-1] = '\0';
		strcpy(author, clientMessage); 
		strcpy(serverCommand, "SEARCHNAME");
	}
	else if(strcmp(serverCommand, "SEARCHNAME") == 0)
	{
		clientMessage[readSize-1] = '\0';
		strcpy(name, clientMessage); 
		strcpy(serverCommand, "SEARCHYEAR");
	}
	else if(strcmp(serverCommand, "SEARCHYEAR") == 0)
	{
		clientMessage[readSize-1] = '\0';
		strcpy(year, clientMessage); 
		
		//searching for artworks
		char *x = (char*)malloc(DEFAULT_BUFLEN*numberOfArtworks*sizeof (char));
		numberOfSearched = 0;
	    	strcat(x, "\nLIST OF SEARCHED ARTWORKS: \n");
		for(int i = 0; i < numberOfArtworks; i++){
			if((strcmp(id, "\0") == 0) || (strcmp(id, artworks[i].id) == 0)) {
				if((strcmp(author, "\0") == 0) || (strcmp(author, artworks[i].author) == 0)) {
					if((strcmp(name, "\0") == 0) || (strcmp(name, artworks[i].name) == 0)) {
						if((strcmp(year, "\0") == 0) || (strcmp(year, artworks[i].year) == 0)) {				    			
				    			strcpy(searchedArtworks[numberOfSearched].id, artworks[i].id);
				    			strcpy(searchedArtworks[numberOfSearched].author, artworks[i].author);
				    			strcpy(searchedArtworks[numberOfSearched].name, artworks[i].name);
				    			strcpy(searchedArtworks[numberOfSearched].year, artworks[i].year);
				    			strcpy(searchedArtworks[numberOfSearched].reserved, artworks[i].reserved);
							numberOfSearched++;
							strcat(x, "\tid:");
							strcat(x, artworks[i].id);
							strcat(x, " AUTHOR:");
							strcat(x, artworks[i].author);
							strcat(x, " NAME:");
							strcat(x, artworks[i].name);
							strcat(x, " YEAR:");
							strcat(x, artworks[i].year);
							strcat(x, "\n");
							
						}
					}
				}
			}
		}
		strcpy(serverMessage, x);
		
		memset(serverCommand, 0, sizeof(serverCommand));
		fflush(stdout);
	}
   	else if(strcmp(clientMessage, "CHECKSTATUS\n") == 0)
   	{
    		if(loggedIn == 0) strcpy(serverMessage, "You are not logged in!");
    		else
    		{
    			if(numberOfSearched == 0) strcpy(serverMessage, "You have not searched for any artwork!\n");
    			else
    			{
	    			char *x = (char*)malloc(DEFAULT_BUFLEN*numberOfArtworks*sizeof (char));
		    		strcat(x, "\nLIST OF SEARCHED ARTWORKS: \n");
	    			for(int i = 0; i < numberOfSearched; i++){
	    				strcat(x, "\tid:");
					strcat(x, searchedArtworks[i].id);
					strcat(x, " AUTHOR:");
					strcat(x, searchedArtworks[i].author);
					strcat(x, " NAME:");
					strcat(x, searchedArtworks[i].name);
					strcat(x, " YEAR:");
					strcat(x, searchedArtworks[i].year);
	    				if(strcmp(searchedArtworks[i].reserved, "-1") == 0)
	    				{
						strcat(x, " RESERVED: NO\n");
	    				}
	    				else
	    				{
	    					strcat(x, " RESERVED: YES\n");
	    				}
	    			}
	    			strcpy(serverMessage, x);
	    		}
    		}

   	}
   	else if(strcmp(serverCommand, "RESERVE") == 0)
    	{
    		int x = 0;
    		clientMessage[readSize-1] = '\0';
    		for(int i = 0; i < numberOfArtworks; i++){
    			if(strcmp(artworks[i].id, clientMessage) == 0)
    			{
    				if(strcmp(artworks[i].reserved, "-1") == 0)
    				{
					strcpy(serverMessage, "Successfully reserved artwork id: ");
					strcat(serverMessage, clientMessage);
					strcat(serverMessage, "\n");
					strcpy(artworks[i].reserved, userID);
					WriteArtworks();
					x = 1;
				}
				
    			}
    		}
    		if(x == 0)
		{
			strcpy(serverMessage, "Unsuccessfully reserved artwork id: ");
			strcat(serverMessage, clientMessage);
			strcat(serverMessage, "\n");
		}
    		for(int i = 0; i < numberOfSearched; i++){
    			if(strcmp(searchedArtworks[i].id, clientMessage) == 0)
    			{
    				if(strcmp(searchedArtworks[i].reserved, "-1") == 0)
    				{
					strcpy(artworks[i].reserved, userID);
				}
    			}
    		}
		strcpy(id, clientMessage); 
			
		memset(serverCommand, 0, sizeof(serverCommand));
    	}
   	else if(strcmp(clientMessage, "RESERVE\n") == 0)
   	{
		if(loggedIn == 0) strcpy(serverMessage, "You are not logged in!");
    		else	
    		{
    			strcpy(serverCommand, "RESERVE");
    		
    		}
    	}
    	else if(strcmp(clientMessage, "LOGOUT\n") == 0)
    	{
    		if(loggedIn == 0) strcpy(serverMessage, "You are not logged in!");
    		else
    		{
    			strcpy(serverMessage, "Logged out!");
    			loggedIn = 0;
    		}
    	}
    	else if(strcmp(clientMessage, "BACK\n") == 0)
    	{
    		memset(serverCommand, 0, sizeof(clientMessage)); 
    	}
    	else if(strcmp(clientMessage, "REGISTER\n") == 0 && strcmp(serverCommand, "LOGIN") == 0)
    	{
    		loginRegister = 2;
    		strcpy(serverCommand, "REG_1"); // forward to the first part of the registration form
    	}
    	else if(strcmp(serverCommand, "LOG_1") == 0)
    	{
    		strcpy(serverCommand, "LOG_2");	// forward to the second part of the login form
    		clientMessage[readSize-1] = '\0';
    		strcpy(usernameOrCardNumber, clientMessage);	
    	}
    	else if(strcmp(serverCommand, "REG_1") == 0)
    	{
    		strcpy(serverCommand, "REG_2");	// forward to the second part of the registration form
    		clientMessage[readSize-1] = '\0';
    		strcpy(usernameOrCardNumber, clientMessage);
    	}
    	else if(strcmp(serverCommand, "LOG_2") == 0 || strcmp(serverCommand, "REG_2") == 0)
    	{
    		clientMessage[readSize-1] = '\0';
    		strcpy(userPassword, clientMessage);	
    		if(loginRegister == 1)	//login selected
    		{
    			for(int i = 0; i < numberOfUsers; i++)
    			{
    				if((strcmp(usernameOrCardNumber, users[i].username) == 0 || strcmp(usernameOrCardNumber, users[i].cardNumber) == 0) && strcmp(userPassword, users[i].password) == 0)
    				{
    					strcpy(serverMessage, "Successfully logged in!");
    					strcpy(userID, users[i].cardNumber);
    					memset(serverCommand, 0, sizeof(serverCommand));	
    					loggedIn = 1;
    					
    				}
    			}
    			if(loggedIn == 0) 
    			{
    				strcpy(serverMessage, "Login failed!");
    				memset(serverCommand, 0, sizeof(serverCommand));
    			}
    		}
    		else if(loginRegister == 2)	// registration selected
    		{
    			strcpy(users[numberOfUsers].username, usernameOrCardNumber);
    			sprintf(users[numberOfUsers].cardNumber, "%d", numberOfUsers);	
    			strcpy(users[numberOfUsers].password, userPassword);
    			
    			WriteUsers();	
    			
    			numberOfUsers++;
    			strcpy(serverMessage, "Successful registration!");
    			memset(serverCommand, 0, sizeof(serverCommand));
    		}
    	}
    	else if(strcmp(clientMessage, "users\n") == 0)
    	{
    		printf("REGISTERED USERS:\n");
    		for(int i = 0; i < numberOfUsers; i++) 
    		{
    			printf("%s %s %s\n", users[i].username, users[i].cardNumber, users[i].password);
    		}
    	}
    	else
    	{
    		strcpy(serverMessage, "Unknown command");
    	}
    
    	if (send(sock, serverMessage, sizeof(serverMessage), 0) < 0){
		printf("Can't send\n");
    	}
    	
    	if (send(sock, serverCommand, sizeof(serverCommand), 0) < 0){
		printf("Can't send\n");
    	}
    
    	memset(clientMessage, 0, sizeof(clientMessage)); 
    }

    if(readSize == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(readSize == -1)
    {
        perror("recv failed");
    }
}

void LoadUsers()
{
	FILE *fp;
	char temp[DEFAULT_BUFLEN];
	int i = 0;
	
	fp = fopen("user_accounts.txt", "a+");
	
	if(fp == NULL){
		printf("Error at opening File!");
		exit(1);
	}
	
	while(fgets(temp, DEFAULT_BUFLEN, fp) != NULL)
	{
		for(int j = 0; j < DEFAULT_BUFLEN; j++)
		{
			if(temp[j] == '\n') temp[j] = 0;
		}
		if(i == 2) 
		{
			strcpy(users[numberOfUsers].password, temp);
			numberOfUsers++;
			i = 0;
		}
		else if(i == 1)
		{
			strcpy(users[numberOfUsers].cardNumber, temp);
			i++;
		}
		else if(i == 0)
		{			
			strcpy(users[numberOfUsers].username, temp);
			i++;
		}
	} 
	
	fclose(fp);
}

void WriteUsers()
{
	FILE *fp;
	fp = fopen("user_accounts.txt", "a");
	
	if(fp == NULL){
		printf("Error at opening File!");
		exit(1);
	}
	
	struct user k = users[numberOfUsers];
	fprintf(fp, "%s\n%s\n%s\n", k.username, k.cardNumber, k.password);
	
	fclose(fp);
}

void LoadArtworks()
{
	FILE *fp;
	char temp[DEFAULT_BUFLEN];
	int i = 0;
	
	fp = fopen("artworks.txt", "a+");
	
	if(fp == NULL){
		printf("Error at opening File!");
		exit(1);
	}
	
	while(fgets(temp, DEFAULT_BUFLEN, fp) != NULL)
	{
		for(int j = 0; j < DEFAULT_BUFLEN; j++)
		{
			if(temp[j] == '\n') temp[j] = 0;
		}
		if(i == 4) 
		{
			strcpy(artworks[numberOfArtworks].reserved, temp);
			numberOfArtworks++;
			i = 0;
		}
		else if(i == 3)
		{
			strcpy(artworks[numberOfArtworks].year, temp);
			i++;
		}
		else if(i == 2)
		{
			strcpy(artworks[numberOfArtworks].name, temp);
			i++;
		}
		else if(i == 1)
		{
			strcpy(artworks[numberOfArtworks].author, temp);
			i++;
		}
		else if(i == 0)
		{			
			strcpy(artworks[numberOfArtworks].id, temp);
			i++;
		}
	} 
	
	
	fclose(fp);
}

void WriteArtworks()
{
	FILE *fp;
	fp = fopen("artworks.txt", "w");
	
	if(fp == NULL){
		printf("Error at opening File!");
		exit(1);
	}
	for(int i = 0; i < numberOfArtworks; i++)
	{
		fprintf(fp, "%s\n%s\n%s\n%s\n%s\n", artworks[i].id, artworks[i].author, artworks[i].name, artworks[i].year, artworks[i].reserved);
	}
	
	fclose(fp);
}


