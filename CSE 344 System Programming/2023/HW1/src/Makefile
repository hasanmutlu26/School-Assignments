compile:
	gcc appendMeMore.c -o appendMeMore -Wall -pthread -g -lm
	gcc duptest.c -o duptest -Wall -pthread -g -lm
	

clean:
	rm appendMeMore
	rm duptest
	rm file.txt

run1: compile
	./appendMeMore f1 1000000 & ./appendMeMore f1 1000000

run2: compile
	./appendMeMore f2 1000000 x & ./appendMeMore f2 1000000 x
