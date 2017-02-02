#
#	This is a sample make file for RTP

CFLAGS = -Wall -Wextra -std=gnu99 -O2 -g
AFLAGS = -D LINUX
SUBMIT = *.c Makefile *.h prj7-server.py

client: client.c rtp.o $(NETWORK_OBJECT)
	gcc $(CFLAGS) -o prj7-client client.c rtp.o
rtp.o : rtp.c rtp.h
	gcc -c $(CFLAGS) $(AFLAGS) rtp.c

submit:	clean
	tar cvfz prj7-submit.tar.gz $(SUBMIT)

clean:
	rm -rf rtp.o prj7-client
