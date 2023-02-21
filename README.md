
#### MIHAJLO KARADŽIĆ

#### FILIP UNČANIN

# eRegister for the Reservation of Artworks


## Main task

```
 Realize an example of a dedicated electronic registry system for
records of issued works of art using the TCP protocol.
 The example consists of a client side (user) and a server side
(electronic register).
```

The client can use the keyboard to set the following commands to be sent to the server:

```
 Login – user login/registration (e.g. via username or number
membership cards and user password)
 Logout – user logout
 Search – search of available contents of the electronic register
 SearchAll – search of all contents of the electronic register
 Search [id:ID] [author:AUTHOR] [truncated art work name:NAME] [year:YEAR] –
search for certain criteria (all or individual)
 CheckStatus – checking the supplied contents of the registered user
 Reserve [id:ID] – reservation of the artwork by the registered user by
ID of the artwork
```

The server sends corresponding responses to the given commands, which are displayed on client application screen.
Notes:

```
 Each artwork in the electronic registry has a unique ID
 Each user of the electronic register receives a unique membership card number during the registration process
```

## The application

```
 The application consists of server.c, artworks.txt and
user_accounts.txt on the server side and client.c on the client side
 First we start the server and then the client
 After the connection is established, a server message is printed to the client
 The client is obliged to register at the very beginning
```

```
 artworks.txt represents a "database" of artworks
 user_accounts.txt represents a ,,database“ of users
```

```
 The "menu" itself on the client side is quite intuitive and well
designed to easily map commands from a task
 Each command opens up a new level and its own unique options
```


## Compiling:
	-server.c and client.c are compiled in separate terminals (with the possibility of opening multiple terminals for client.c, i.e. multiple clients)
	-is done using the "make server" and "make client" commands
	-deleting executable files is done with "make server_clean" and "make client_clean"

## Startup:
	-starting is done with the commands "./server" which is started first and then "./client"
	-to exit the application, use CTRL+C
