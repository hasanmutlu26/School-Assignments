C = 10
N = 100
I = input.txt

compile:
	gcc hw4.c -o hw4 -Wall -pthread -g

run:
	./hw4 -C $(C) -N $(N) -F $(I)

crun: compile run

frun: file compile run

frun2: file2 compile run

valgrind: file compile
	valgrind -v --trace-children=yes --track-fds=yes --track-origins=yes --leak-check=full --show-leak-kinds=all ./hw4 -C $(C) -N $(N) -F $(I)

clean:
	rm hw4

file:
	gcc filecreater/filecreater.c -o filecreater/filecreater -Wall
	./filecreater/filecreater $(I) $(C) $(N)

file2:
	gcc filecreater/filecreater2.c -o filecreater/filecreater2 -Wall
	./filecreater/filecreater2 $(I) $(C) $(N) 