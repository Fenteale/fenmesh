src = $(wildcard *.c)
obj = $(src:.c=.o)

LDFLAGS =

.PHONY: all
all: fenmesh-client

fenmesh-client: main.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(obj) fenmesh-client