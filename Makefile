src = $(wildcard *.c)
obj = $(src:.c=.o)

LDFLAGS =

.PHONY: all
all: fenmesh-client

.PHONY: all-debug
all-debug: override CFLAGS += -Wall -g
all-debug: all

fenmesh-client: main.c readconfig.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(obj) fenmesh-client