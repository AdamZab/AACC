#include <Wire.h>  
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <EEPROM.h>

#define YELLOWBUTTONPIN 2
#define BLACKBUTTONPIN 3
#define REDBUTTONPIN 4
#define RELAYPIN 7
#define SENSOR1PIN 8
#define SENSOR2PIN 9
#define SENSOR3PIN 10
#define TEMPERATURE_RESOLUTION 1
#define HUMIDITY_RESOLUTION 5
#define BUFFOR 2
#define DEGREES_SIGN 223

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
DHT sensor1;
DHT sensor2;
DHT sensor3;

int redButtonState;
int blackButtonState;
int yellowButtonState;
int relayState;
bool backLight = true;
bool printScreenFlag = false;
String mode;
signed char bufforMode = 0; // -2 : AC OFF; 2 : AC ON; 0 : buffor OFF 
int addrSetMode = 0;
int addrSetTemperature = 1;
int addrSetHumidity = 2;
int setMode = EEPROM.read(0);
int setTemperature = EEPROM.read(1);
int setHumidity = EEPROM.read(2);

void setup() {
    lcd.begin(16,2);
    lcd.backlight();
    lcd.setCursor(0,0);
    lcd.print("AIR CONDITIONING");
    lcd.setCursor(5,1);
    lcd.print("CONTROL");
    sensor1.setup(SENSOR1PIN);
    sensor2.setup(SENSOR2PIN);
    sensor3.setup(SENSOR3PIN);
    pinMode(RELAYPIN, OUTPUT);
    digitalWrite(RELAYPIN, HIGH);
    pinMode(REDBUTTONPIN, OUTPUT);
    pinMode(BLACKBUTTONPIN, OUTPUT);
    pinMode(YELLOWBUTTONPIN, OUTPUT);
    delay(5000);
    lcd.clear();
}

int readTemperature(){
    int temperature1 = sensor1.getTemperature();
    int temperature2 = sensor2.getTemperature();
    int temperature = (temperature1+temperature2)/2;
    return temperature;
}

int readHumidity(){
    int humidity1 = sensor1.getHumidity();
    int humidity2 = sensor2.getHumidity();
    int humidity = (humidity1+humidity2)/2;
    return humidity;
}

void printFirstLine(int temperatureIn,int temperatureOut, int humidity){
    lcd.setCursor(0,0);
    lcd.print(temperatureIn);
    lcd.print((char) DEGREES_SIGN);
    lcd.print("C ");
    lcd.print(humidity);
    lcd.print("% | ");
    lcd.print(temperatureOut);
    lcd.print((char) DEGREES_SIGN);
    lcd.print("C ");
}

String changeMode(int temperature, int humidity){
    if (setMode == 3)
        setMode = 0;
        
    if (redButtonState == HIGH){
        setMode++;
        EEPROM.update(0, setMode);
    }
  
    if (setMode == 0){
        lcd.setCursor(0,1);
        lcd.print("OFF  HUMI:");
        lcd.print(setHumidity);
        lcd.print("%   ");
        digitalWrite(RELAYPIN, HIGH);
        bufforMode = 0;
        if (blackButtonState == HIGH && yellowButtonState == LOW) {
            setHumidity = setHumidity - HUMIDITY_RESOLUTION;
            EEPROM.update(2, setHumidity);
        }

        if (yellowButtonState == HIGH && blackButtonState == LOW) {
            setHumidity = setHumidity + HUMIDITY_RESOLUTION;
            EEPROM.update(2, setHumidity);
        }
    }
    else if (setMode == 1){
        lcd.setCursor(0,1);
        lcd.print("ON   HUMI:");
        lcd.print(setHumidity);
        lcd.print("%   ");
        digitalWrite(RELAYPIN, LOW);
        bufforMode = 0;
    }
    else if (setMode == 2){
        lcd.setCursor(0,1);
        if (humidity > setHumidity + bufforMode){
            digitalWrite(RELAYPIN, LOW);
            lcd.print("ON   ");
        }
        else if (setTemperature - bufforMode <= temperature){
            digitalWrite(RELAYPIN, LOW);
            lcd.setCursor(0,1);
            lcd.print("ON   ");
            bufforMode = 1;
        }
        else if (setTemperature + bufforMode > temperature){
            digitalWrite(RELAYPIN, HIGH);
            lcd.setCursor(0,0);
            lcd.print("OFF  ");
            bufforMode = -1;
        }

        lcd.print("AUTO:");
        lcd.print(setTemperature);
        lcd.print((char) DEGREES_SIGN);
        lcd.print("C  ");
     
        
        if (blackButtonState == HIGH && yellowButtonState == LOW) {
            setTemperature = setTemperature - TEMPERATURE_RESOLUTION;
            EEPROM.update(1, setTemperature);
        }

        if (yellowButtonState == HIGH && blackButtonState == LOW) {
            setTemperature = setTemperature + TEMPERATURE_RESOLUTION;
            EEPROM.update(1, setTemperature);
        }
    }
    return mode;
    
}

void changeLight(){
    if (blackButtonState == HIGH && yellowButtonState == HIGH){
          if (backLight == true){
              lcd.setBacklight(LOW);
              backLight = false;
          }
          else {
              lcd.setBacklight(HIGH);
              backLight = true;
          }
    }
}

void loop() {
    redButtonState = digitalRead(REDBUTTONPIN);
    blackButtonState = digitalRead(BLACKBUTTONPIN);
    yellowButtonState = digitalRead(YELLOWBUTTONPIN);
    relayState = digitalRead(RELAYPIN);
    int temperatureIn = readTemperature();
    int temperatureOut = sensor3.getTemperature();
    int humidity = readHumidity();
    changeMode(temperatureIn, humidity);
    changeLight();
    printFirstLine(temperatureIn, temperatureOut, humidity);
    delay(200);
}
