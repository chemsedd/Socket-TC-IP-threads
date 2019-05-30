#ifndef CLIENT_HEADERS
#define CLIENT_HEADERS


//  --------------- LIBRARIES ----------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#define PORT 4444                   //  Connection port
#define BUFFER_SIZE 256             //  Message to send size

int clientSocket;                   //  Client socket
struct sockaddr_in serverAddr;      //  Socket address
char buffer[BUFFER_SIZE] = "0";     //  Message to send


// ----- Functions declarations

void init_sock_addr(struct sockaddr_in *address);
void getMessage(char *string);
void exit_error(char* er);

#endif // !CLIENT_HEADERS