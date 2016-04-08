#include <stdbool.h>
#include <wiringPi.h>

void putLed ( unsigned int pins[], bool values[], unsigned int );
int readBtn ( unsigned int btn[],  unsigned int );
int sendValues ( unsigned int sendPins[], unsigned int, bool values[], unsigned int, unsigned int );
int recvValues ( unsigned int recvPins[], unsigned int, bool * values, unsigned int, unsigned int );


