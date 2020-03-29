#include <Wire.h>  
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

#define YELLOWBUTTONPIN 2
#define BLACKBUTTONPIN 3
#define REDBUTTONPIN 4
#define RELAYPIN 7
#define SENSOR1PIN 8
#define SENSOR2PIN 9
#define RESOLUTION 1

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
DHT sensor1;
DHT sensor2;

int redButtonState;
int blackButtonState;
int yellowButtonState;
int relayState;
int counter = 0;
int setTemperature = 20;
bool backLight = true;
bool printScreenFlag = false;
String mode;

void setup() {
    lcd.begin(16,2);
    lcd.backlight();
    lcd.setCursor(0,0);
    lcd.print("AIR CONDITIONING");
    lcd.setCursor(5,1);
    lcd.print("CONTROL");
    sensor1.setup(SENSOR1PIN);
    sensor2.setup(SENSOR2PIN);
    pinMode(RELAYPIN, OUTPUT);
    pinMode(REDBUTTONPIN, OUTPUT);
    pinMode(BLACKBUTTONPIN, OUTPUT);
    pinMode(YELLOWBUTTONPIN, OUTPUT);
    delay(5000);
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

void printScreen(int temperature, int humidity, String mode){
    lcd.setCursor(4,0);
    lcd.print(temperature);
    lcd.print((char) 223);
    lcd.print("C ");
    lcd.print(humidity);
    lcd.print("%");
    lcd.setCursor(0,1);
    lcd.print(mode); 
    if (counter == 2){
        lcd.print(setTemperature);
        lcd.print((char) 223);
        lcd.print("C");
    }
}

String changeMode(int temperature){
    if (counter == 3)
        counter = 0;
        
    if (redButtonState == HIGH)
        counter++;
  
    if (counter == 0){
        mode = "       OFF";
        digitalWrite(RELAYPIN, HIGH);
    }
    else if (counter == 1){
        mode = "       ON";
        digitalWrite(RELAYPIN, LOW);
    }
    else if (counter == 2){
        mode = "Automatic: ";
        if (setTemperature < temperature){
            digitalWrite(RELAYPIN, LOW);
            lcd.setCursor(0,0);
            lcd.print("ON ");
        }
        else if (setTemperature >= temperature){
            digitalWrite(RELAYPIN, HIGH);
            lcd.setCursor(0,0);
            lcd.print("OFF");
        }
        
        if (blackButtonState == HIGH && yellowButtonState == LOW) {
            setTemperature = setTemperature - RESOLUTION;
        }

        if (yellowButtonState == HIGH && blackButtonState == LOW) {
            setTemperature = setTemperature + RESOLUTION;
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
    lcd.clear();
    redButtonState = digitalRead(REDBUTTONPIN);
    blackButtonState = digitalRead(BLACKBUTTONPIN);
    yellowButtonState = digitalRead(YELLOWBUTTONPIN);
    relayState = digitalRead(RELAYPIN);
    int temperature = readTemperature();
    int humidity = readHumidity();
    changeMode(temperature);
    changeLight();
    printScreen(temperature, humidity, mode);
    delay(200);
}
