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

#define PORTNUM     6017

static void daemonize() {
    pid_t pid;

    pid = fork();

    if (pid < 0) {
        printf("Daemonization failed!\n");
        exit(EXIT_FAILURE);
    }
    
    if (pid > 0)
        exit(EXIT_SUCCESS);

    if (setsid() < 0)
        exit(EXIT_FAILURE);
    
    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);

    pid = fork();

    if (pid < 0)
        exit(EXIT_FAILURE);
    
    if (pid > 0)
        exit(EXIT_SUCCESS);

    umask(0);

    chdir("/");

    int x;
    for(x = sysconf(_SC_OPEN_MAX); x>=0; x--)
        close(x);

}


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