#include "client.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "config/readconfig.h"
#include "log/log.h"

#define PORTNUM     6017 //temp

void setupClient() {
    if (parseConfig("fenmesh.conf"))
        logWrite("%s", getParseError());

    const char* host_ipVal = getConfigValue("host_ip");
    if (host_ipVal==NULL) {
        logWrite("%s", getParseError());
        exit(1);
    }
    else
        logWrite("Host_ip value: %s", host_ipVal);
    // initialize our variables
    int socket_d;
    struct sockaddr_in server_a;
    char read_buffer[512];

    socket_d = socket(AF_INET, SOCK_STREAM, 0); // open a socket
    if (socket_d < 0) {
        logWrite("Could not open socket: %s", strerror(errno));
        exit(1);
    }

    server_a.sin_addr.s_addr = inet_addr(host_ipVal); // set server's ip (in this case, the same PC)
    server_a.sin_family = AF_INET; 
    server_a.sin_port = htons(PORTNUM); //set port number

    if (connect(socket_d, (struct sockaddr *)&server_a, sizeof(server_a)) < 0) { //try to connect to server
        logWrite("Connection failed.");
        exit(1);
    }
    logWrite("Connected to %s", host_ipVal); // success!

    if( recv(socket_d, read_buffer, 512, 0) < 0) { //try to recieve a message thats 512 bytes long and store it in read_buffer
        logWrite("Error reading from server.");
        exit(1);
    }

    //printf("%s\n", read_buffer); //print out message recieved to terminal
    
    close(socket_d);
}