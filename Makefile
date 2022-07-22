srcdir := src

src := $(shell find $(srcdir) -name '*.c')
incdir := $(shell find $(srcdir) -type d)
incflags := $(addprefix -I,$(incdir))
obj = $(src:.c=.o)

LDFLAGS =

.PHONY: all
all: fenmesh

.PHONY: all-debug
all-debug: override CFLAGS += -Wall -g
all-debug: all

fenmesh: $(src)
	$(CC) $(incflags) $(CFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(obj) fenmesh