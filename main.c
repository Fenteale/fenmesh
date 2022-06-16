//######################################################
// Fenmesh Client
//######################################################

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORTNUM     6017
#define SERVER_IP   "127.0.0.1" //server is running on same computer

int main() {
    // initialize our variables
    int socket_d;
    struct sockaddr_in server_a;
    char read_buffer[512];

    socket_d = socket(AF_INET, SOCK_STREAM, 0); // open a socket
    if (socket_d < 0) {
        fprintf(stderr, "Could not open socket: %s\n", strerror(errno));
        exit(1);
    }

    server_a.sin_addr.s_addr = inet_addr(SERVER_IP); // set server's ip (in this case, the same PC)
    server_a.sin_family = AF_INET; 
    server_a.sin_port = htons(PORTNUM); //set port number

    if (connect(socket_d, (struct sockaddr *)&server_a, sizeof(server_a)) < 0) { //try to connect to server
        fprintf(stderr, "Connection failed.\n");
        exit(1);
    }
    printf("Connected to " SERVER_IP "\n"); // success!

    if( recv(socket_d, read_buffer, 512, 0) < 0) { //try to recieve a message thats 512 bytes long and store it in read_buffer
        fprintf(stderr, "Error reading from server.\n");
        exit(1);
    }

    printf("%s\n", read_buffer); //print out message recieved to terminal
    
    close(socket_d);

    return 0;
}