all : server client

server.o: server.c pokemon.h
	gcc -c server.c

client.o: client.c pokemon.h
	gcc -c client.c

common.o common.c pokemon.h
	gcc -c common.c

server: server.o common.o
	gcc -o server server.o common.o

client: client.o common.o
	gcc -o client client.o common.o

clean:
	rm -f * .o server client
