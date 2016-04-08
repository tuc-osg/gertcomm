#include <stdlib.h>
#include <stdio.h>
#include "mylib.h"


/**
 * \brief puts the GPIO-pins into a state
 * \param[in] pins array of the pins to be set
 * \param[in] values array of the values to be set to the pins
 * \param[in] length the number of pins to set
 * \warn length of pins and values must be at least length!
 **/
void putLed ( unsigned int pins[], bool values[], unsigned int length )
{
	for (int i=0; i<length; i++)
	{
		digitalWrite ( pins[i], ((values[i] == true) ? HIGH : LOW) );
	}
}

/**
 * \brief reads GPIO-pin-values and returns them as int (32bit)
 * \param[in] btn array of pins to be read
 * \param[in] length the number of pins to be read
 * \return bitmap of the pins (beginning with 0x01, 0x02, ...)
 **/
int readBtn (unsigned int btn[], unsigned int length )
{
	int result = 0;
	int mask = 0x01;
	for (int i=0; i<length; i++)
	{
		result = result | (digitalRead ( btn[i] ) == HIGH) ? mask : 0;
		mask = mask << 1;
	}
	return result;
}

/**
 * \brief sends the databits to the receiver, using the sendPins
 * \param[in] sendPins array of pins to be used for sending bits to the receiver
 * \param[in] readyPin will be flipped, when the sender is done changing the bits around
 * \param[in] values array of values to be send
 * \param[in] length the length of the data-word
 * \param[in] numPins the number of pins for beeing able to send
 * \note length should be dividable by numPins without rest!
 * \return 0 
 **/
int sendValues ( unsigned int sendPins[], unsigned int readyPin, bool values[], unsigned int length, unsigned int numPins )
{
	/** 13 zeroes **/
	for (int i=0; i<13; i++)
	{
		for (int x=0; x<numPins; x++)
		{
			digitalWrite ( sendPins[x], LOW );
		}
		bool a = ((digitalRead ( readyPin ) == HIGH) ? true : false);
		digitalWrite (readyPin, ((a==true) ? LOW : HIGH));
		delay(5);
	}

	/** one zero **/
	for (int i=0; i<numPins; i++)
	{
		digitalWrite ( sendPins[i], HIGH );
	}
	bool a = ((digitalRead ( readyPin ) == HIGH) ? true : false);
	digitalWrite (readyPin, ((a==true) ? LOW : HIGH));
	delay(5);

	digitalWrite (readyPin, ((a==true) ? HIGH : LOW));
	delay(5);

	int pos = 0;
	for (; pos<length; pos++)
	{
		digitalWrite (sendPins[0], ((values[pos]==true) ? HIGH : LOW ));
		a = ((digitalRead ( readyPin ) == HIGH) ? true : false);
		digitalWrite (readyPin, ((a==true) ? LOW : HIGH));
		delay(5);
	}

	for (int i=0; i<numPins; i++)
	{
		digitalWrite ( sendPins[i], LOW );
	}
	a = ((digitalRead ( readyPin ) == HIGH) ? true : false);
	digitalWrite (readyPin, ((a==true) ? LOW : HIGH));
	return 0;
}

int recvValues ( unsigned int recvPins[], unsigned int readyPin, bool* values, unsigned int length, unsigned int numPins )
{
	enum 
	{
		INTER_FRAME_GAP, 
		INTER_FRAME_GAP1,
		FRAME,
		END
	} state = INTER_FRAME_GAP;
	
	int readyState = ((digitalRead ( readyPin ) == HIGH) ? true : false);
	int zeroes = 0;
	int ones = 0;
	int pos = 0;
	while (state != END)
	{
		bool rstate = ((digitalRead ( readyPin ) == HIGH) ? true : false);
		if (rstate != readyState)
		{
			readyState = rstate;
			bool val = ((digitalRead ( recvPins[0] ) == HIGH) ? true : false);
			switch (state)
			{
				case INTER_FRAME_GAP:
					if (val == false)
					{
						zeroes++;
						if (zeroes>13)
						{
							state = INTER_FRAME_GAP1;
							ones = 0;
							zeroes = 0;
						}
					}
					else zeroes=0;
					break;
				case INTER_FRAME_GAP1:
					if (val == true)
					{
						ones++; 
						if (ones >= 2)
						{
							state = FRAME;
							ones = 0;
						}
					}
					else
					{
						ones = 0;
					}
					break;
				case FRAME:
					if (val == true)
					{
						values[pos] = true;
					}
					else
					{
						values[pos] = false;
					}
					pos++;
					if (pos >= length)
						state = END;
					break;
				default:
					state = END;
					break;
			}
		}
		//delay(1);	
	}
	return 0;
}
