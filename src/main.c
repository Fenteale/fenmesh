//######################################################
// Fenmesh Client
//######################################################

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "log.h"
#include "client.h"
#include "server.h"
#include "readconfig.h"
#include "daemon.h"

#define PORTNUM     6017

int main(int argc, char *argv[]) {
    int option;
    int toDaemonize = 0;
    int clientOrServer = 0; //0 for server, 1 for client
    while((option = getopt(argc, argv, "dc")) != -1){
        switch(option) {
            case 'd':
                printf("Launching as Daemon.\n");
                toDaemonize = 1;
                break;
            case 'c':
                printf("Launching as client.\n");
                clientOrServer = 1;
                break;
            default:
                printf("Unknown option %c\n", option);
                break;
        }
    }

    if (parseConfig("fenmesh.conf"))
        printf("%s", getParseError());

    const char* log_path = getConfigValue("log_path");
    if (log_path==NULL) {
        printf("%s", getParseError());
        exit(1);
    }
    else
        printf("Log path value: %s\n", log_path);

    if(toDaemonize) {
        openLog(log_path, 1);
        start_daemon_socket();
        daemonize();
    }
    else {
        openLog(log_path, 0);
    }

    if(clientOrServer)
        setupClient();
    else
        setupServer();

    
    logWrite("Fenmesh-server shutting down.");
    closeLog();
    

    return EXIT_SUCCESS;
}