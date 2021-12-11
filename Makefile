all: server_o server_link

server_o:
	gcc -Wall -g -lpthread -c src/cli.c src/invidx.c src/linkedlist.c src/util.c src/server.c

server_link:
	gcc -o server cli.o invidx.o linkedlist.o util.o server.o

clear:
	rm *.o
	rm server
