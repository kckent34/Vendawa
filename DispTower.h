#ifndef DispTower_h
#define DispTower_h

#include <Mux.h>

#define MOTB_EN_PIN 2
#define MOTF_EN_PIN 3
#define SENSB_PIN A1
#define SENSF_PIN A0
#define TOWER_EN0 44
#define TOWER_EN1 45
#define TOWER_EN2 46
#define TOWER_EN3 49
#define TOWER_EN4 48



class DispTower
{
public:
	DispTower(int towerNum, String medName, int medCount);
	int dispense(int speedB, int speedF, int delayTime);
	int medCount();
	int enableTower();
	int disableTower();
	int testSensors();
	int testMotors();
	void printSensorAvg();

private:
	int _medCount;
	String _medName;
	Mux _sensMuxB;
	Mux _sensMuxF;
	Mux _motMuxF;
	Mux _motMuxB;
	int _s0Val;
	int _s1Val;
	int _s2Val;
	int _towerNum;
	int _towerEnPin;
	int _towerOn;
	int _cnt;
	int _avgFront;
	int _avgBack;

};

#endif