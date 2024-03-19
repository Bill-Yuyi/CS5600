all:
	make compile
	mkdir disk
	make run
compile:
	gcc node.c message.c LRU.c main.c -o cache
run:
	./cache
clean:
	rm ./cache
	rm -r disk