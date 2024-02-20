all:
	make compile
	make run
compile:
	gcc mainProgram.c queue.c genRand.c -o mainProgram -lpthread
	gcc encodeMain.c table.c polybius.c -o cipherProgram
	chmod +x cipherProgram
run:
	./mainProgram words.txt
clean:
	rm -rf outputs
	rm ./words.txt
	rm ./cipherProgram
	rm ./mainProgram