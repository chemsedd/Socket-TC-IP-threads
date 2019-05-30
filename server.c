//  Libraries
#include "server_headers.h"

/**
*   Main program for Server
*/
void main(){
   
    int new_occ;

    //  Creating Socket
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        exit_error("## ERROR creating socket");

    //  Setting up address
    init_sock_addr(&serverAddr);

    //  Assigning Address to Socket
    if(bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
        exit_error("## ERROR binding socket with server\n");

    if(listen(sockfd, 3) < 0)
        exit_error("## ERROR waiting for clients\n");

    printf("+---------------------------------------------+\n");
    printf("|                Server                       |\n");
    printf("+---------------------------------------------+\n");

    char input[10];
    do {
        //  Reading user input
        printf(">> Define character to look for: ");
        getMessage(input);
        //  In case user typed 
        if(strlen(input) > 1)
            printf("-- Please type one character (a-z) or (A-Z)!!\n");
    } while(strlen(input) > 1);
    
    target = input[0];

    printf("+------------\n");
    printf("- Waiting for clients...\n");
    printf("+------------\n");

    pthread_mutex_init(&lock, NULL);
    
    do {
        //  Listening (waiting) for message from client
        if((newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size)) < 0)
            exit_error("## ERROR accepting messages\n");
        clients[clients_count] = newSocket;
        recv(newSocket, buffer, BUFFER_SIZE, 0);
        
        new_occ = count(buffer, target);
        printf("\t> Client %2d: %-22s (%c:%2d)\n", clients_count, buffer, target, new_occ);
        sprintf(toSend, "Your message contains %2d: '%c'!", new_occ, target);
        send(newSocket, toSend, BUFFER_SIZE, 0);
        update_max(new_occ);
        clients_count++;
    }while(clients_count < 3);
    
    //  Closing socket
    close(sockfd);


    if(max_occ == 0)
        sprintf(toSend, "#No occurence was found!!");
    else
        sprintf(toSend, "%s -> (%c:%2d)", maxMessage, target, max_occ);
    
    //  Sending results to all clients
    for(int i = 0; i < 3; i++)
        send(clients[i], toSend, BUFFER_SIZE, 0);
    
    //  Closing connections with all clients
    for(int i = 0; i < 3; i++)
        close(clients[i]);
    printf("+------------\n");
    if(max_occ != 0)
        printf("> Message is: %s\n", toSend);
    else
        printf("> %s\n", toSend);
    printf("+------------\n");
    //  Closing socket
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
 * 
 * */
void update_max(int new_occurence) {
    if(MAX(new_occurence, max_occ) == new_occurence) {
        max_occ     = new_occurence;
        toSend[0]   = '\0';
        strcpy(maxMessage, buffer);
    }
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
 *  Calculating the occurance of a char in a string
 * */
int count(const char *string, char c) {
    int temp = 0;

    for (int i = 0; i < strlen(string); i++)
        if (string[i] == c)
            temp++;
    return temp;
}

/**
 *  Printing Error & Exit
 * */
void exit_error(char* er) {
    perror(er);
    exit(-1);
}


void *serve_client_func(void *sock) {

    int *clientSocket = sock;   //  Clients Socket
    char buffer[BUFFER_SIZE];
    char sending[BUFFER_SIZE];   //  clients 

    //  adding client to the list
    //add_client(pthread_self(), *clientSocket);
    //  Receive Client's message
    recv(*clientSocket, buffer, BUFFER_SIZE, 0);
    //  Calculate occurance
    pthread_mutex_lock(&lock);
    int new_occ = count(buffer, target);
    printf("> Client %2d: %-22s (%c:%2d)\n", clients_count, buffer, target, new_occ);
    sprintf(sending, "Your message contains %2d: '%c'!", new_occ, target);
    pthread_mutex_unlock(&lock);
    //  Send client results
    send(*clientSocket, sending, BUFFER_SIZE, 0);
    pthread_mutex_lock(&lock);
    update_max(new_occ);
    clients_count++;
    pthread_mutex_unlock(&lock);
    pthread_exit(EXIT_SUCCESS);
}