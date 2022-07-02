src = $(wildcard *.c)
obj = $(src:.c=.o)

LDFLAGS =

.PHONY: all
all: fenmesh

.PHONY: all-debug
all-debug: override CFLAGS += -Wall -g
all-debug: all

fenmesh: main.c readconfig.c client.c server.c log.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(obj) fenmesh