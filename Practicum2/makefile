all: server client

server: server.c
	gcc handle_request.c file_io.c server.c -o server

client: client.c
	gcc file_io.c client.c -o client
