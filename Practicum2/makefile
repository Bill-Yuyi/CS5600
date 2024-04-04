all: server client

server: server.c
	gcc global_mutex.c handle_request.c file_io.c server.c -o server -pthread

client: client.c
	gcc file_io.c client.c -o client
