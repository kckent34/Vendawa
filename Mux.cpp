#include "Mux.h"


void Mux::init(int type, int sigPin)
{

	_type = type; // either 0 -> sensors  or 1-> motors 
	_sigPin = sigPin; // signal pin... sensors -> analogInput , motors -> pwm output
	// _towerNum = towerNum;
	_speed = 0;





  	if(_type == 0){
  		pinMode(_sigPin,INPUT);
  	} else if (_type == 1) {
  		pinMode(_sigPin,OUTPUT);
  	}

  	// if(_towerNum == 0){
  	// 	_s0Val = 0;
  	// 	_s1Val = 0;
  	// 	_s2Val = 0;
  	// } else if(_towerNum == 1){
  	// 	_s0Val = 1;
  	// 	_s1Val = 0;
  	// 	_s2Val = 0;
  	// } else if(_towerNum == 2){
  	// 	_s0Val = 0;
  	// 	_s1Val = 1;
  	// 	_s2Val = 0;
  	// } else if(_towerNum == 3){
  	// 	_s0Val = 1;
  	// 	_s1Val = 1;
  	// 	_s2Val = 0;
  	// } else if(_towerNum == 4){
  	// 	_s0Val = 0;
  	// 	_s1Val = 0;
  	// 	_s2Val = 1;
  	// }


}

// int Mux::prepareRead(){
// 	if(_type == 0) {
// 		digitalWrite(S0_PIN,_s0Val); 
// 		digitalWrite(S1_PIN,_s1Val);
// 		digitalWrite(S2_PIN,_s2Val);
// 		return 0;
// 	} else {
// 		return 1;
// 	}
// }



int Mux::readMux()
{
	if (_type == 0){
		return analogRead(_sigPin);
	} else {
		return -1;
	}

}

int Mux::setSpeed(int speed){
	_speed = speed;
	return 0;
}



// int Mux::prepareWrite(){
// 	if(_type == 1){
// 		digitalWrite(S0_PIN,_s0Val); 
// 		digitalWrite(S1_PIN,_s1Val);
// 		digitalWrite(S2_PIN,_s2Val);
// 		return 0;
// 	}else {
// 		return 1;
// 	}

// }

int Mux::writeMux()
{
	// speed -> 0 to 255
	if(_type == 1){
		analogWrite(_sigPin,_speed);
		return 0;
	}else {
		return 1;
	}

}

int Mux::stopWrite(){ // shut off motors 

	if(_type == 1){
		analogWrite(_sigPin,0);
		return 0;
	}else {
		return 1;
	}

}











