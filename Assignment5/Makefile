all:
	make compile
	make run
compile:
	gcc mainProgram.c queue.c genRand.c -o mainProgram -lpthread
	gcc encodeMain.c table.c polybius.c -o cipher_program
	chmod +x cipher_program
run:
	mkdir outputs
	./mainProgram test.txt
clear:
	rm -rf outputs
	rm ./cipher_program
	rm ./mainProgram