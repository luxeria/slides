CCFLAGS ?= -Wall -O2 -D_GNU_SOURCE -luring
all_targets = io_uring_echo_server

.PHONY: liburing

all: $(all_targets)

clean:
	rm -f $(all_targets)

liburing:
	+$(MAKE) -C ./liburing

io_uring_echo_server:
	$(CC) io_uring_echo_server.c -o ./io_uring_echo_server -I./liburing/src/include/ -L./liburing/src/  ${CCFLAGS}
