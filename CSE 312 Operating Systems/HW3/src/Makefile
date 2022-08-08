BLOCKSIZE = 16
FILEPATH = mySystem.dat
OPER = dir
PARAM = /

compile:
	gcc makeFileSystem.c -o makeFileSystem -Wall
	gcc fileSystemOper.c -o fileSystemOper -Wall

clean:
	rm makeFileSystem
	rm fileSystemOper

run: compile
	./makeFileSystem $(BLOCKSIZE) $(FILEPATH)

oprun: compile run
	./fileSystemOper $(FILEPATH) $(OPER) "$(PARAM)"

valgrind: compile
	valgrind -v --trace-children=yes --track-fds=yes --track-origins=yes --leak-check=full --show-leak-kinds=all ./makeFileSystem $(BLOCKSIZE) $(FILEPATH)

opvalgrind: compile run 
	valgrind -v --trace-children=yes --track-fds=yes --track-origins=yes --leak-check=full --show-leak-kinds=all ./fileSystemOper $(FILEPATH) $(OPER) "$(PARAM)"