#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "mylib.h"

#define LENGTH 12

int main()
{
	if (wiringPiSetup() == -1)
	{
		printf ("Intialisierung fehgeschlagen; breche ab\n");
		exit ( EXIT_FAILURE );
	}

	unsigned int pins[12] = { 6, 5, 4, 3, 2, 1, 0, 16, 15, 14, 12, 13 };  
	bool nightrider[12] = {0,0,0,0,0,0,0,0,0,1,1,1};
	bool binary[12] = {false};
	bool fill[12] = {false};
	bool* values = fill;
	
	unsigned int sendPins[1] = { 8 };
	unsigned int readyPin = 9;

	int state = 0;
	unsigned int btn[3] = {7, 11, 10};

	for (int i=0; i<4; i++)
	{
		pinMode ( sendPins[i], OUTPUT );
		digitalWrite ( sendPins[i], LOW );
	}
	pinMode ( readyPin, OUTPUT );
	digitalWrite ( readyPin, LOW );

	for (int i=0; i<LENGTH; i++)
	{
		pinMode ( pins[i], OUTPUT );
		digitalWrite ( pins[i], LOW );
	}

	for (int i=0; i<3; i++)
	{
		pinMode ( btn[i], INPUT );
	}

	int a = 0;
	unsigned int num = 0;
	int ncurrent = 3;
	int ndirection = 1;
	while (1)
	{
		if (digitalRead ( btn[0] ) == LOW)
		{
			state = 0;
			values = fill;
		}
		else if (digitalRead ( btn[1] ) == LOW)
		{
			values = binary;
			state = 1;
		}
		else if (digitalRead ( btn[2] ) == LOW)
		{
			state = 2;
			values = nightrider;
		}

		switch (state)
		{
			/** binary counting **/
			case 1:
			{
				int mask = 0x01;
				for (int i=0; i<12; i++)
				{
					values[i] = ((num & mask) > 0 ? true : false);
					mask = mask << 1;
				}

				num++;
				if (num > 4095) num = 0;
				break;
			}
			/** nightrider mode **/
			case 2:	
				if (ncurrent >= 11 && ndirection > 0)
					ndirection = -1;
				if (ncurrent < 3 && ndirection < 0)
					ndirection = 1;
				if (ndirection == 0) ndirection = 1;

				for (int i=0; i<12; i++)
				{
					nightrider[i] = false;
				}

				for (int i=0; i<3; i++)
				{
					nightrider[ncurrent-i] = true;	
				}

				ncurrent = ncurrent + ndirection;
				break;

			/** fill mode **/
			default: 
				fill[a] = !fill[a];
				a = (a+1) % 12;
				break;
		}

		// sending the data to the other pi
		for (int i=0; i<5; i++)
		{
			sendValues ( sendPins, readyPin, values, LENGTH, 1 );
			putLed ( pins, values, 12 );
		}

	}
	return 0;
}

