all:
	make compile
	make run
compile:
	gcc pqueuepmain.c -o mainProgram -lpthread
run:
	./mainProgram
clean:
	rm -rf outputs
	rm ./words.txt
	rm ./cipherProgram
	rm ./mainProgram