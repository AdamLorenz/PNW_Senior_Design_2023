#include <SD.h>                  //SD Card Reader Library
#include <DS3231.h>              //Clock Module Library
#include <LiquidCrystal_I2C.h>   //LCD Display Module Library

File dataFile;                      //Data File Object
DS3231 rtc;                         //Clock Object
LiquidCrystal_I2C lcd(0x27, 16, 2); //LCD Display Object

// CLOCK SETTINGS
bool centuryBit = false;
bool h12 = false; 
bool hPM = false; 

// PIN ASSIGNMNETS
int anemometerPin = A0; 
int upperLoadPin = A2;    
int lowerLoadPin = A1;
int upperPWMPin = 9;
int lowerPWMPin = 0;

// LOAD RESISTANNCE
double loadResistance = 1.3; // [Ohms]

String labels = "month\tdate\thour\tmin\tsec\tupperP\tlowerP\ttotalP\twind(mph)";  // Data column labels
double dutyCycleIncrement = 1.25; // Increment used to increase and decrease duty cycle of power controller

// Returns voltage at a given input pin
double getPinVoltage(int pin){
  return analogRead(pin) * (5.0/1024.0);
}

// Returns the power dissipated by a load
double getLoadPower(int pinLoad){
  return sq(getPinVoltage(pinLoad) * 5)/loadResistance;
}

// Returns a string of the current time stamp: month, date, hour, minute, and second
String getTimeStamp(){
  byte month = rtc.getMonth(centuryBit);
  byte date = rtc.getDate();
  byte hr = rtc.getHour(h12, hPM);
  byte min = rtc.getMinute();
  byte sec = rtc.getSecond();
  String TimeStamp = "";
  TimeStamp.concat(month);
  TimeStamp.concat("\t");
  TimeStamp.concat(date);
  TimeStamp.concat("\t");
  TimeStamp.concat(hr);
  TimeStamp.concat("\t");
  TimeStamp.concat(min);
  TimeStamp.concat("\t");
  TimeStamp.concat(sec);
  TimeStamp.concat("\t");
  return TimeStamp;
}

// Returns current wind speed
double getWindSpeed(){
  return (getPinVoltage(anemometerPin)*45/5 ) * 2.23694; // m/s converted to mph (1 m/s ~= 2.23694 mph)
}

// Writes a string to a line of "SVAWTData.txt" on the SD card
void writeSD(String string){
  dataFile = SD.open("VAWTData.txt", FILE_WRITE); //File name no longer than 8 characters
  dataFile.println(string);
  dataFile.close();
}

// class Anemometer{

//}

class PWM{
  public:
    int pin;
    double dutyCycle = 0;
    double dCIncrement = dutyCycleIncrement;

    // Sets a PWM to a given duty cycle
    void setPWM(double dc){
      dutyCycle = dc;
      analogWrite(pin, round(255*dc/100));
    }
};

class Turbine{
  public:
    double power;
    PWM pwm;
    int load;
    double newPower = -1;

    adjustLoad(){
      power = newPower;
      newPower = sq((pwm.dutyCycle/100)*getPinVoltage(load) * 5)/loadResistance;
      if((pwm.dutyCycle + pwm.dCIncrement > 0) & (pwm.dutyCycle + pwm.dCIncrement < 100)){ // Limit pwm dutycycle to between 0 and 100
        if(newPower < power){
          pwm.dCIncrement *= -1;
        }else{
          pwm.setPWM(pwm.dutyCycle + pwm.dCIncrement);
        }
      }else{
        pwm.dCIncrement *= -1;
      }
    }
};

PWM upperPWM;
PWM lowerPWM;
Turbine upperTurbine;
Turbine lowerTurbine;

String getData(){
      String dataString = "";
      double utp = upperTurbine.power;
      double ltp = lowerTurbine.power;
      dataString.concat(utp);
      dataString.concat("\t");
      dataString.concat(ltp);
      dataString.concat("\t");
      dataString.concat(utp + ltp);
      dataString.concat("\t");
      dataString.concat(getWindSpeed());
      return dataString;
}

void updateLCD(String d){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("UP:");
    lcd.print(int(upperTurbine.power)); // Upper Turbine Power
    lcd.print(" TotP:");
    lcd.print(int(upperTurbine.power + lowerTurbine.power)); // Total Power
    lcd.setCursor(0, 1);
    lcd.print("LP:");
    lcd.print(int(lowerTurbine.power)); // Lower Turbine Power
    lcd.print(" WSpd:");
    lcd.print(int(getWindSpeed())); // Wind Speed
}

// runs once at the start of the program
void setup() { 
  //Set PWM trigger pins
  upperPWM.pin = upperPWMPin;
  upperPWM.setPWM(0);
  lowerPWM.pin = lowerPWMPin;
  lowerPWM.setPWM(0);
  
  //Set Turbine members
  upperTurbine.pwm = upperPWM;
  lowerTurbine.pwm = lowerPWM;
  upperTurbine.load = upperLoadPin;
  lowerTurbine.load = lowerLoadPin;

  //LCD Display Initialization
  lcd.init();
  lcd.clear();
  lcd.backlight();

  //Serial Monitor Initialization (used for debugging)
  Serial.begin(9600);
  while(!Serial){
    ;
  }
  
  //SD Card Reader Initialization
  SD.begin(4);
  Serial.println(labels);
  writeSD(labels);
}

int i = 0;
int j = 0;
// loops endlessly
void loop() {
  
  upperTurbine.adjustLoad();
  //Serial.print(upperTurbine.power); Serial.print(" "); Serial.println(upperTurbine.newPower);
  //Serial.println(upperTurbine.pwm.dutyCycle);
  //lowerTurbine.adjustLoad();

  i++;
  if(i == 100){ // Writes data to SD card every 100th iteration (10 seconds)
    String timeStamp = getTimeStamp();
    String data = getData();
    //Serial.println(timeStamp + data);
    Serial.println(timeStamp + data);// Serial.println(upperTurbine.pwm.dutyCycle);
    writeSD(timeStamp + data);
    updateLCD(data);
    i = 0;
  } 

  delay(100); // Loops approximately every 10th of a second
}

