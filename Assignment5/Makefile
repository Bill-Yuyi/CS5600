all:
	make compile
	make run
compile:
	gcc genRand_yuyi.c queue.c -o genRand -lpthread
	gcc encodeMain.c table.c polybius.c -o cipher_program
	chmod +x cipher_program
run:
	./genRand test.txt
	