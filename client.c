//  Libraries
#include "client_headers.h"

/**
*   Main program for Client
*/
void main(){
    

    //  Creating Socket
    if((clientSocket = socket(PF_INET, SOCK_STREAM, 0)) < 0)
        exit_error("## ERROR creating socket");
 
    //  Setting Socket Address
    init_sock_addr(&serverAddr);

    //  Connecting address with socket
    if(connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
        exit_error("## ERROR connecting");

    printf("+---------------------------------------------+\n");
    printf("|               Client                        |\n");
    printf("+---------------------------------------------+\n");
    
    //  Sending a message to the server
    printf("> Type a message to send: ");
    getMessage(buffer);
    send(clientSocket, buffer, BUFFER_SIZE, 0);

    //  Receiving a message from the server
    char msg[BUFFER_SIZE];
    recv(clientSocket, msg, BUFFER_SIZE, 0);

    //  Printing results
    printf("+----------------------------------------------\n");
    printf("> Server => %-22s\n", msg);
    printf("+------------\n");
    recv(clientSocket, msg, BUFFER_SIZE, 0);
    printf("> Server => %-22s .\n", msg);
    printf("+----------------------------------------------\n");
    //  Closing connection
    close(clientSocket);
    printf("- Closing connection...\n\n");
}


//  --------------- FUNCTIONS ----------------
/**
 *  Initializing socket address
 * */
void init_sock_addr(struct sockaddr_in *address) {
    memset(address, '0', sizeof(*address));
    (*address).sin_family       = AF_INET;                  //  TCP
    (*address).sin_addr.s_addr  = inet_addr("127.0.0.1");   //  Localhost
    (*address).sin_port         = htons(PORT);              //  Communication port
}

/**
 *  Reading a user input (string)
 * */
void getMessage(char *string) {
    fgets(string, BUFFER_SIZE, stdin);
    fflush(stdin);
    char *c;
    if ((c = strchr(string, '\n')))
        *c = '\0';
}

/**
 *  Printing Error & Exit
 * */
void exit_error(char* er) {
    perror(er);
    exit(-1);
}