#ifndef SERVER_HEADERS
#define SERVER_HEADERS


//  --------------- LIBRARIES ----------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#define PORT 4444                   //  Connection port
#define BUFFER_SIZE 256             //  Message to send size


#define MAX(a, b) ((a > b) ? a : b) //  Max function (int)
int sockfd;                         //  Socket id
int clients[3];                     //  Clients sockets
struct sockaddr_in serverAddr;      //  Socket address
int newSocket;                      //  Received message socket
int clients_count = 0;              //  Number of clients
struct sockaddr_in newAddr;         //  Client address
socklen_t addr_size;                //  Size of the Address

char target = 'x';                  //  Char to look for
char buffer[BUFFER_SIZE] = "";      //  Buffer to store data
char toSend[BUFFER_SIZE];
char maxMessage[BUFFER_SIZE];
int max_occ = 0;

pthread_mutex_t lock;

// ----- Functions declarations

void init_sock_addr(struct sockaddr_in *address);
void update_max(int new_occurence);
void getMessage(char *string);
int count(const char *string, char c);
void exit_error(char* er);
void *serve_client_func(void *sock);


#endif // !SERVER_HEADERS