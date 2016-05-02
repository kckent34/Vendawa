#include "Adafruit_FONA.h"
#include "Keypad.h"
#include "LiquidCrystal_I2C.h"
#include "Wire.h"
#include "SoftwareSerial.h"
#include <ArduinoJson.h>
//#include "EEPROMex.h"
#include "Timer.h"


//FONA CONSTANTS 
#define FONA_TX 10 // check this 
#define FONA_RX 2 
#define FONA_RST 4
#define FONA_RI 18 // possible interupts: 2,3,18,19,20,21
#define FONA_WAIT 5000 // wait time (milliseconds) until querying for new sms

//LCD CONSTANTS 
#define LCD_I2C_ADDR 0x27

//Motor Arduino I2C address 
#define  MOTORS_I2C_ADDR 8

//KEYPAD CONSTANTS 
#define KROW1_PIN 39
#define KROW2_PIN 45
#define KROW3_PIN 47
#define KROW4_PIN 43
#define KCOL1_PIN 37
#define KCOL2_PIN 35
#define KCOL3_PIN 41

#define CODE_LENGTH 5




// KEYPAD GLOBALS  
bool enteringCode = false;
int keyCount = 0;
const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] =
 {{'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}};
 
byte rowPins[ROWS] = {KROW1_PIN, KROW2_PIN, KROW3_PIN, KROW4_PIN}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {KCOL1_PIN, KCOL2_PIN, KCOL3_PIN}; // connect to the column pinouts of the keypad

Keypad pad = Keypad(makeKeymap(keys),rowPins,colPins,ROWS,COLS);

//LCD GLOBALS
LiquidCrystal_I2C lcd(LCD_I2C_ADDR,20,4);

//FONA GLOBALS
char replybuffer[255]; // buffer for replies 
SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX);
Adafruit_FONA fona = Adafruit_FONA(FONA_RST);
//volatile int address = 0;

//Meds 
String med0 = String("doxycycline"); //tower 0 
String med1 = String("ibuprofen"); // tower 1
String med2 = String("amoxicillin"); // tower 2 
String med3 = String("tamiflu"); // tower 3 
String med4 = String("carbatrol"); // tower 4

char data[500][5];
//volatile int addresses[500];
int cnt = 0;
uint8_t startAddress = 1;



//const int memBase = 300;
//const int maxAllowedWrites = 500;

//timer
Timer t;


void setup() 
{
  //Start up wire
  Wire.begin();
  

  
  //setup timer events 
  int tickEvent = t.every(20000,checkText);
 
  
 
//  EEPROM.setMemPool(memBase, EEPROMSizeMega);
//  EEPROM.setMaxAllowedWrites(maxAllowedWrites);
  Serial.begin(115200);

  //turn on lcd and turn screen off at first 
  lcd.init(); 
  lcd.backlight();
  lcd.noDisplay();
  
  pad.addEventListener(keypadEvent);

//Uncomment to try interrupt 
//  pinMode(FONA_RI,INPUT);
//  digitalWrite(FONA_RI,HIGH);
//  attachInterrupt(digitalPinToInterrupt(FONA_RI),checkText,FALLING);
  
  while (!fonainit()) { delay(5000);}
  // Print SIM card IMEI number.
  char imei[15] = {0}; // MUST use a 16 character buffer for IMEI!
  uint8_t imeiLen = fona.getIMEI(imei);
  if (imeiLen > 0) {
    Serial.print("SIM card IMEI: "); Serial.println(imei);
  }

}

void loop() {
mainCode();
}


void mainCode(){

  t.update();
  pad.getKey();
  if(enteringCode){
    char code[5];
    code[0] = pad.waitForKey();
    code[1] = pad.waitForKey();
    code[2] = pad.waitForKey();
    code[3] = pad.waitForKey();
    code[4] = pad.waitForKey();
    lcd.print(code[4]);
    if(code[4] == '#'){
      char towerNum = checkForId(code);
      if(towerNum == 'n'){
        lcdPrintMainScreen();
      } else{
        sendDispense(towerNum);
      }
    }  
    enteringCode = false;
  }
  
}

char checkForId(char* code){
//  int cnter = 0
//  int output[2];
  char* temp;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Searching for code..");
  for(int i = 0; i < 500; i++){
    temp = data[i];
    if(strncmp(code,temp,4) == 0){
        lcd.setCursor(5,0);
        lcd.print("Valid Code");
        char copy = temp[4];
        data[i][0] = 'a';
        data[i][1] = 'b';
        data[i][2] = 'c';
        data[i][3] = 'd';
        data[i][4] = 'e';
        return copy; // return the tower Number with the right meds 
    } 
  }
  lcd.print("Invalid Code");
  return 'n';
  
    
//    EEPROM.readBlock(address, output,2);

//    if(output[0] == codeNum){
//      lcd.clear();
//      lcd.setCursor(0,0);
//      lcd.print("Invalid Code");
//      break;
//    }
    
//  }

}

boolean fonainit(void) {
  Serial.println(F("Initializing....(May take 3 seconds)"));
  // make it slow so its easy to read!
  fonaSS.begin(4800); // if you're using software serial
  //Serial1.begin(4800); // if you're using hardware serial
 
  // See if the FONA is responding
  if (! fona.begin(fonaSS)) {// can also try fona.begin(Serial1) 
    Serial.println(F("Couldn't find FONA"));
    return false;
  }
  Serial.println(F("FONA is OK"));
  return true;
}


void keypadEvent(KeypadEvent key){
    switch (pad.getState())
    {
    //U can use three modes: PRESSED, HOLD or RELEASED.
    //U can also add more than one event with more than one key.
    case PRESSED:
        if ( (key != NO_KEY) & enteringCode == false){
          enteringCode = true;
          lcdPrintMainScreen();
          break;
        } 

        if (key != NO_KEY & enteringCode == true){
          keyCount++;
          if(keyCount < CODE_LENGTH){
              lcd.print(key);
          }else if(keyCount == CODE_LENGTH){
              lcd.print(key);
              keyCount = 0;
              enteringCode == false;
          }
          
          
        }
        break;

  }
}



void checkText() {
  lcdPrintUpdatingTextScreen();
  int8_t smsnum = fona.getNumSMS();
  if (smsnum < 0) {
    Serial.println(F("Could not read # SMS"));
    return;
  } else {
    Serial.print(smsnum); 
    Serial.println(F(" SMS's on SIM card!"));
  }
  
  if (smsnum == 0) { 
    lcd.clear();
    lcd.noDisplay();
    return;
  }

   uint8_t n = 1; 
   for(int j = 1; j <= 10; j++){
     uint16_t smslen;
     char sender[25];
     uint8_t len = fona.readSMS(n, replybuffer, 250, &smslen); // pass in buffer and max len!
     // if the length is zero, its a special case where the index number is higher
     // so increase the max we'll look at!
     if (len == 0) {
        Serial.println(F("[empty slot]"));
        n++;
        continue;
     }
     if (! fona.getSMSSender(n, sender, sizeof(sender))) {
       // failed to get the sender?
       sender[0] = 0;
     }
      //parse the incoming text
        String str1 = String(replybuffer);
        str1.replace("\\","");
        const char *jsonString2 = str1.c_str();
        StaticJsonBuffer<255> jsonBuffer;
        JsonObject& object = jsonBuffer.parseObject(jsonString2);
     
        if(!object.success()){
          Serial.println("Failed to parse, Invalid text");
          fona.deleteSMS(n);
          Serial.println("Deleted invalid text from fona");
        
        
        } else

        {
        //Parse the object
        const char* first = object["first"];
        const char* last = object["last"];
        const char* med = object["med"];
        const char* id = object["id"];
        char towerChar;
        Serial.print(first);
        Serial.print(last);
        Serial.print(med);
        Serial.print(id);
    
        
//        int store[2];
        if(med0.equalsIgnoreCase(med)){
          towerChar = '0';
        }else if (med1.equalsIgnoreCase(med)){
          towerChar = '1';
        }else if(med2.equalsIgnoreCase(med)){
          towerChar = '2';
        }else if(med3.equalsIgnoreCase(med)){
          towerChar = '3';
        }else if(med4.equalsIgnoreCase(med)){
          towerChar = '4';
        } else {
          Serial.println("Failed to find tower for medication: ");
          Serial.println(med);
        }
        Serial.print("Adding ID and towerNum to data: ");
        Serial.print(id);
        Serial.print(towerChar);
        data[cnt][0] = id[0];
        data[cnt][1] = id[1];
        data[cnt][2] = id[2];
        data[cnt][3] = id[3];
        data[cnt][4] = towerChar;
        cnt++;
        
        fona.deleteSMS(n);
        Serial.println("deleted text from fona");
        
      }
   
  }
  startAddress = startAddress + 10;
  lcdPrintMainScreen();
  
}


void lcdPrintMainScreen() {
    lcd.clear();
    lcd.display();
    lcd.setCursor(0,0);
    lcd.print("Welcome to APM");
    lcd.setCursor(0,1);
    lcd.print("Please Enter Code:");
    lcd.setCursor(0,2);
}

void lcdPrintUpdatingTextScreen() {
  lcd.clear();
  lcd.display();
  lcd.setCursor(0,0);
  lcd.print("One moment....");
  lcd.setCursor(0,1);
  lcd.print("Updating database");
}

int sendDispense(char towerNum){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Dispensing...");
  Serial.println("Sending");
  Serial.println(towerNum);
  Wire.beginTransmission(MOTORS_I2C_ADDR);
  Wire.write(towerNum);
  Wire.endTransmission();
  lcd.clear();
  lcd.noDisplay();
  return 0;
  
}

               





