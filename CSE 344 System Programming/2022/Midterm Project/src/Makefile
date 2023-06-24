compile:
	gcc client.c -o client -Wall -lrt -lm -g
	gcc serverY.c -o serverY -lm -Wall -lrt 
	gcc serverZ.c -o serverZ -lm -Wall -lrt

yps:
	ps -C serverY -o "pid ppid pgid sid tty command"

zps:
	ps -C serverZ -o "pid ppid pgid sid tty command"

ps:
	ps -C serverY -o "pid ppid pgid sid tty command"
	ps -C serverZ -o "pid ppid pgid sid tty command"

runclient: compile
	./client -s /tmp/svy_sv -o input.csv

runmulticlient: compile
	./client -s /tmp/svy_sv -o input.csv 
	sleep 0.5
	./client -s /tmp/svy_sv -o input2.csv 
	sleep 0.5
	./client -s /tmp/svy_sv -o input.csv 
	sleep 0.5
	./client -s /tmp/svy_sv -o input3.csv 

runmulclient: compile
	./client -s /tmp/svy_sv -o input.csv 
	./client -s /tmp/svy_sv -o input2.csv 
	./client -s /tmp/svy_sv -o input3.csv 
	./client -s /tmp/svy_sv -o input4.csv 
	./client -s /tmp/svy_sv -o input.csv 
	./client -s /tmp/svy_sv -o input2.csv 
	./client -s /tmp/svy_sv -o input3.csv 
	./client -s /tmp/svy_sv -o input4.csv 
	
runservery: compile
	./serverY -s /tmp/svy_sv -o yout.log -p 10 -r 2 -t 2


run: compile runservery yps runclient

valgrind_servery:
	valgrind --track-origins=yes ./serverY -s /tmp/svy_sv -o yout.log -p 5 -r 5 -t 2 

valgrind_client:
	valgrind ./client -s /tmp/svy_sv -o input.csv --track-origins=yes


