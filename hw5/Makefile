TARGETSRV = guess-server
TARGETCLT = guess-client

CFLAGS = -Wall -Wextra --std=gnu99 -g -O2 -MD -MP
SRCSSRV = io.c numbers.c server.c
SRCSCLT = io.c client.c

all: $(TARGETSRV) $(TARGETCLT)

$(TARGETSRV): $(SRCSSRV:%.c=%.o)
	$(CC) $(CFLAGS) -o $@ $^

$(TARGETCLT): $(SRCSCLT:%.c=%.o)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGETSRV) $(SRCSSRV:%.c=%.o) $(SRCSSRV:%.c=%.d)
	rm -f $(TARGETCLT) $(SRCSCLT:%.c=%.o) $(SRCSCLT:%.c=%.d)