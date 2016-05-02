#ifndef Mux_h
#define Mux_h

#include "Arduino.h"

#define S0_PIN 37
#define S1_PIN 35
#define S2_PIN 33

class Mux
{
public:
	void init(int type, int sigPin);	
	int readMux();
	int writeMux();
	int stopWrite();
	int setSpeed(int speed);

private:
	int _sigPin;
	int _type;
	// int _towerNum;
	// int _s0Val;
	// int _s1Val;
	// int _s2Val; 
	int _speed; 

};

#endif