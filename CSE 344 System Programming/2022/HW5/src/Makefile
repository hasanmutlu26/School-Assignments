N = 5
M = 6
I = input1.txt
J = input2.txt
O = out.txt

compile:
	gcc hw5.c -o hw5 -Wall -pthread -g -lm

run: compile 
	./hw5 -i $(I) -j $(J) -o $(O) -n $(N) -m $(M)

frun: file compile run

clean:
	rm hw5

file:
	gcc filegenerator/filegenerator.c -o filegenerator/filegenerator -lm
	./filegenerator/filegenerator $(I) $(J) $(N) $(M)


valgrind: file compile
	valgrind -v --trace-children=yes --track-fds=yes --track-origins=yes --leak-check=full --show-leak-kinds=all ./hw5 -i $(I) -j $(J) -o $(O) -n $(N) -m $(M)