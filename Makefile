.PHONY: server
server: server
	./server

.PHONY: client
client: client
	./client


all: client.c server.c 
	gcc -o client client.c
	gcc -o server server.c
	 
clean:
	rm client sever

