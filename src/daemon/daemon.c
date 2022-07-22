#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <signal.h>
#include <string.h>
#include <stddef.h>
#include <errno.h>

#include "daemon.h"
#include "config.h"

void daemonize() {
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

void start_daemon_socket() {
    struct stat st;
    int sockf_status = stat(DAEMON_SOCKET_NAME, &st);
    if (sockf_status == 0) {
        //file exists already
        if ((st.st_mode & __S_IFMT) == __S_IFSOCK) {
            //and it is a socket
            sockf_status = unlink(DAEMON_SOCKET_NAME);
            if (sockf_status != 0) {
                printf("Couldn't unlink socket.\n");
                exit(EXIT_FAILURE);
            }
        } else {
            printf("The file " DAEMON_SOCKET_NAME " already exists and is not a socket.\n");
            exit(EXIT_FAILURE);
        }
    }

    int socket_d = socket(AF_LOCAL, SOCK_STREAM, 0);
    if (socket_d < 0) {
        printf("Error creating daemon socket.\n");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_un name;
    name.sun_family = AF_LOCAL;
    strncpy(name.sun_path, DAEMON_SOCKET_NAME, sizeof(name.sun_path));
    name.sun_path[sizeof(name.sun_path) - 1] = '\0';

    size_t size = (offsetof (struct sockaddr_un, sun_path)
          + strlen (name.sun_path));

    if(bind (socket_d, (struct sockaddr *) &name, size) < 0) {
        printf("Couldn't bind daemon socket: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
}