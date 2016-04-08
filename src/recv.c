#include <stdio.h>
#include <stdlib.h>

#include "mylib.h"
#include <wiringPi.h>

#define LENGTH 12

int main ()
{
	if (wiringPiSetup() == -1)
	{
		printf ("Initialisierung fehlgeschlagen, breche ab\n");
		exit ( EXIT_FAILURE );
	}

	unsigned int pins[12] = { 6, 5, 4, 3, 2, 1, 0, 16, 15, 14, 12, 13 };
	bool values[12] = {false};
	unsigned int recvPins[1] = { 8 };
	unsigned int readyPin = 9;

	for (int i=0; i<4; i++)
	{
		pinMode ( recvPins[i], INPUT );
	}
	pinMode ( readyPin, INPUT );
	
	for (int i=0; i<LENGTH; i++)
	{
		pinMode ( pins[i], OUTPUT );
		digitalWrite ( pins[i], LOW );
	}

	while (1)
	{
		recvValues ( recvPins, readyPin, values, LENGTH, 1 );

		putLed ( pins, values, LENGTH );
	}

	return 0;
}

