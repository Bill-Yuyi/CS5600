all: server client

server: server.c handle_request.c global_mutex.c file_io.c table.c polybius.c
	gcc table.c polybius.c handle_request.c global_mutex.c file_io.c server.c -o server -pthread

client: client.c file_io.c table.c polybius.c
	gcc table.c polybius.c file_io.c client.c -o client
