# Fenmesh
Fenmesh is a heavily WIP system that can be used to monitor and keep track of systems remotely from a central master server.

Although Fenmesh is not ready for use, some basic features have already been implemented. The following will show an example usage with currently working commands.

### First edit config file

The file `fenmesh.conf` holds all configuration options for the program. In order to have the client connect to the master server the host ip must be supplied. An example `fenmesh.conf` is in the repo which used the IP `127.0.0.1` as the master server IP.

### Start the Masterserver

To start the masterserver, run fenmesh with no options, or with the `-d` flag to have it run as a daemon.

```fenmesh -d```

### Run the client

The client can be run with the `-c` flag. An example is as follows.

```fenmesh -c```

If the connection was successful, then the IP of the client will be printed to the server's console. If the server is running as a daemon, then it will be added to the file defined in `log.h` as `TEMP_LOG_NAME`.

## To-Do List
This list is non-exhaustive.

- Allow fenmesh program to interface with daemon if it is already running.
- Continue to add more features and make the program overall more robust.
- Consider using libyaml instead of custom config code.