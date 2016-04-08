all:
	gcc -lwiringPi send.c mylib.c -std=c99 -Wall -o send
	gcc -lwiringPi recv.c mylib.c -std=c99 -Wall -pedantic -o recv
	gcc -lwiringPi -lpthread buttons.c mylib.c -std=c99 -Wall -pedantic -o buttons
