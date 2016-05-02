#include "DispTower.h"


DispTower::DispTower(int towerNum, String medName,int medCount)
{
	//initialize class variables 
	// *_sensMuxB = Mux(0,SENSB_PIN,towerNum);
	// *_sensMuxF = Mux(0,SENSF_PIN,towerNum);
	// *_motMuxF = Mux(1,MOTF_EN_PIN,towerNum);
	// *_motMuxB = Mux(1,MOTB_EN_PIN,towerNum);
	_sensMuxB.init(0,SENSB_PIN);
	_sensMuxF.init(0,SENSF_PIN);
	_motMuxF.init(1,MOTF_EN_PIN);
	_motMuxB.init(1,MOTB_EN_PIN);

	_medName = medName;
	_medCount = medCount;
	_towerNum = towerNum;
	_towerOn = 0;
	_cnt = 0;
	_avgFront = 0;
	_avgBack = 0;

	if(_towerNum == 0){
  		_s0Val = 0;
  		_s1Val = 0;
  		_s2Val = 0;
  		_towerEnPin = TOWER_EN0;
  	} else if(_towerNum == 1){
  		_s0Val = 1;
  		_s1Val = 0;
  		_s2Val = 0;
  		_towerEnPin = TOWER_EN1;
  	} else if(_towerNum == 2){
  		_s0Val = 0;
  		_s1Val = 1;
  		_s2Val = 0;
  		_towerEnPin = TOWER_EN2;
  	} else if(_towerNum == 3){
  		_s0Val = 1;
  		_s1Val = 1;
  		_s2Val = 0;
  		_towerEnPin = TOWER_EN3;
  	} else if(_towerNum == 4){
  		_s0Val = 0;
  		_s1Val = 0;
  		_s2Val = 1;
  		_towerEnPin = TOWER_EN4;
  	}

	// SENS and MOT pins get setup in mux class 



}

int DispTower::dispense(int speedB, int speedF, int delayTime)
{

	// Mux _sensMuxB = Mux(0,SENSB_PIN,0);
	// Mux _sensMuxF = Mux(0,SENSF_PIN,0);
	// Mux _motMuxF = Mux(1,MOTF_EN_PIN,0);
	// Mux _motMuxB = Mux(1,MOTB_EN_PIN,0);
	//speeds between 0-255
	//delay time in milliseconds 

	if(_towerOn == 0){
		enableTower();
	}
	

	//initialize variables  
	int  avgFront = 0;
	int  avgBack = 0;
	int valFront = 0;
	int valBack = 0;
	int threshBack = 500;
	int threshFront =  500;

	//set motor speeds 
	_motMuxB.setSpeed(speedB); 
	_motMuxF.setSpeed(speedF);

	if(_cnt == 0){
		for(int i = 0; i < 20; i++){
			//init sensors;
			avgBack += _sensMuxB.readMux();
			avgFront += _sensMuxF.readMux();
		}
			_avgFront  = avgFront / 20;
			_avgBack = avgBack  / 20;

	}

	_cnt ++;

	Serial.println(_avgFront);
	Serial.println(_avgBack);
	// // initialize sensors 
	// for(int i = 0; i < 20; i++){
	// 	_avgBack += _sensMuxB.readMux();
	// 	_avgFront += _sensMuxF.readMux();
	// }
	// _avgFront  = _avgFront / 20;
	// _avgBack = _avgBack  / 20;

	

	valBack = _sensMuxB.readMux(); // read back sensor 
	valFront = _sensMuxF.readMux();
	// Serial.println(_avgBack);
	// while(Serial.available() == 0){}
	_motMuxB.writeMux();// turn ON back motor

	while(valBack > (_avgBack - threshBack)){ 
		valBack = _sensMuxB.readMux();
		// Serial.println(valBack);
							 }
	// delay(delayTime);

	// _motMuxB.stopWrite(); // turn OFF the back motor 
	// // while(Serial.available() == 0){}

	// Serial.read();

	_motMuxF.writeMux(); // turn ON front motor 


	
	valFront = _sensMuxF.readMux();
	while(valFront >= (_avgFront - threshFront)){ 
		valBack = _sensMuxB.readMux();
		valFront = _sensMuxF.readMux();
		if(valBack < (_avgBack - threshBack)){
			_motMuxB.writeMux();
		} else{
			_motMuxB.stopWrite();
		}
		// Serial.println(valFront);
	} 

	_motMuxF.writeMux();
	//front sensor covered 
	while(valFront <=(_avgFront - threshFront)) {
		valFront = _sensMuxF.readMux();
		Serial.println(valFront);
		_motMuxF.writeMux();
	}

	// delay(1000);
	// front sensor uncovered 
	_motMuxF.stopWrite(); // stop front motor 

	_medCount = _medCount - 1; // update medcine count for this tower onces dispensed 

	disableTower(); // turn off power to tower boards 

	Serial.println("Done dispensing");

	// while(Serial.available() == 0){};
	// Serial.read();

	return 0;




}

int DispTower::medCount(){return _medCount;}


int DispTower::enableTower(){
	//turn on power to tower and set the select pins 
	if(_towerOn == 0){
		digitalWrite(_towerEnPin,LOW);
		digitalWrite(S0_PIN,_s0Val); 
		digitalWrite(S1_PIN,_s1Val);
		digitalWrite(S2_PIN,_s2Val);
		_towerOn = 1;
		return 0;
	} else {
		return 1;
	}

}

int DispTower::disableTower(){
	//turn off power to board 
	if(_towerOn == 1){
		digitalWrite(_towerEnPin,HIGH);
		_towerOn = 0;
		return 0;
	} else {
		return 1;
	}

}



int DispTower::testSensors(){

	if(_towerOn == 0) {
		enableTower();
	}

	int sigF = 0;
	int sigB = 0;

	while(Serial.available() == 0){
		sigB = _sensMuxB.readMux();
		sigF = _sensMuxF.readMux();
		Serial.print("sigF: ");
  		Serial.print(sigF);
  		Serial.print("       sigB: ");
  		Serial.println(sigB);
	}

	disableTower();
	return 0;
}

int DispTower::testMotors()
{
	_motMuxF.setSpeed(200);
	_motMuxB.setSpeed(200);


	Serial.println("Press anything to test Front motor");
	while(Serial.available() == 0){};
	if(_towerOn == 0) {
		enableTower();
	}
	while(Serial.available() == 1){
		
		_motMuxF.writeMux();
	}

	_motMuxF.stopWrite();
	Serial.read();
	Serial.read();

	Serial.println("Press anything to test Back motor");
	while(Serial.available() == 0){};
	while(Serial.available() == 1){
		_motMuxB.writeMux();
	}

	_motMuxB.stopWrite();
	Serial.read();
	Serial.read();

	disableTower();



}

void DispTower::printSensorAvg(){
	Serial.print("Front Avg: ");
	Serial.print(_avgFront);
	Serial.print("        Back Avg:");
	Serial.println(_avgBack);

}








