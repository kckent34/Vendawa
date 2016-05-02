#include "DispTower.h"
#include "Wire.h"

// S0_PIN = 33
// S1_PIN = 35
// S2_PIN = 37

//MOTB_EN_PIN = 1 
//MOTF_EN_PIN = 2
//SENSB_PIN = A0
//SENSF_PIN = A1

// TOWER_EN0 = 45
// TOWER_EN1 = 47
// TOWER_EN2 = 49
// TOWER_EN3 = 51
// TOWER_EN4 = 53
DispTower dispTower0 = DispTower(0, "doxycycline", 100);
DispTower dispTower1 = DispTower(1, "ibuprofen", 100);
DispTower dispTower2 = DispTower(2, "amoxicillin", 100);
DispTower dispTower3 = DispTower(3, "tamiflu", 100);
DispTower dispTower4 = DispTower(4, "carbatrol", 100);
int tower = 0;
bool dispense = false;

void setup() {
  //set up all pins 
  pinMode(S0_PIN,OUTPUT);
  pinMode(S1_PIN,OUTPUT);
  pinMode(S2_PIN,OUTPUT);
  pinMode(MOTB_EN_PIN,OUTPUT);
  pinMode(MOTF_EN_PIN,OUTPUT);
  pinMode(SENSB_PIN,INPUT);
  pinMode(SENSF_PIN,INPUT);
  pinMode(TOWER_EN0,OUTPUT);
  pinMode(TOWER_EN1,OUTPUT);
  pinMode(TOWER_EN2,OUTPUT);
  pinMode(TOWER_EN3,OUTPUT);
  pinMode(TOWER_EN4,OUTPUT);
  digitalWrite(TOWER_EN0,HIGH);
  digitalWrite(TOWER_EN1,HIGH);
  digitalWrite(TOWER_EN2,HIGH);
  digitalWrite(TOWER_EN3,HIGH);
  digitalWrite(TOWER_EN4,HIGH);

//  setup wire for i2c
  Wire.begin(8);
  Wire.onReceive(receiveEvent);
  
//  
  Serial.begin(9600);
//  while(Serial.available() == 0){};
//  dispTower3.dispense(200,100,500);
}


void loop() {
  
  delay(100);

//
//  dispTower2.testSensors();
  if(dispense){
    if(tower == 0) { dispTower0.dispense(200,200,500);}
    else if(tower == 1) {dispTower1.dispense(200,200,500);}
    else if(tower == 2) {dispTower2.dispense(200,200,500);
                                            Serial.println("IN 2");}
    else if(tower == 3) {dispTower3.dispense(200,100,500);}
    else if(tower == 4) {dispTower4.dispense(200,100,500);}
    tower = -1;
    dispense == false;
  }
    

}



//
void receiveEvent(int howMany) 
{
  char towerNum;
  while(Wire.available() > 0){
    towerNum = Wire.read();
    Serial.println(towerNum);
  }

  if(towerNum == '0'){
    dispense = true ;
    tower = 1;
//    dispTower0.dispense(200,100,500);
  }else if(towerNum == '1'){
//    dispTower2.dispense(200,100,500);
    Serial.println("in here");
    dispense = true;
    tower = 2;
  }else if(towerNum == '2'){
//    dispTower2.dispense(200,100,500);
    dispense = true;
    tower =2 ;
  }else if(towerNum == '3'){
    dispense = true;
    tower= 1;
  }else if(towerNum == '4'){
//    dispTower0.dispense(200,100,500);
    dispense = true;
    tower = 1;
  }
  return;
  
  
}


