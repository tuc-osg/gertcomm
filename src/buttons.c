#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "mylib.h"

#include <pthread.h>

#define LENGTH 12

void* blink ( void* readyPin )
{
	unsigned int rpin = *(unsigned int*) readyPin;
	while (1)
	{
		bool val = ((digitalRead ( rpin )==HIGH) ? true : false);
		digitalWrite (rpin, (val==true) ? LOW : HIGH);
		delay(500);
	}
}

int main()
{
	if (wiringPiSetup() == -1)
	{
		printf ("Intialisierung fehgeschlagen; breche ab\n");
		exit ( EXIT_FAILURE );
	}

	unsigned int btn[3] = {6,5,4};
	unsigned int sendPins[3] = {8,9,7};

	unsigned int readyPin = 0;

	pinMode ( readyPin, OUTPUT );
	digitalWrite ( readyPin, LOW );

	pthread_t mythread;
	pthread_create ( &mythread, NULL, &blink, (void*) &readyPin );		
	
	for (int i=0; i<3; i++)
	{
		pinMode ( sendPins[i], OUTPUT );
		digitalWrite ( sendPins[i], LOW );

		pinMode ( btn[i], INPUT );
		pullUpDnControl ( btn[i], PUD_UP );
	}

	while (1)
	{
		digitalWrite ( sendPins[0], digitalRead ( btn[0] ));
		digitalWrite ( sendPins[1], digitalRead ( btn[1] ));
		digitalWrite ( sendPins[2], digitalRead ( btn[2] ));
		delay (50);
	}
	return 0;
}

