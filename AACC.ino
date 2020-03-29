#include <Wire.h>  
#include <LiquidCrystal_I2C.h>
#include <SimpleDHT.h>

#define YELLOWBUTTONPIN = 2;
#define BLACKBUTTONPIN = 3;
#define REDBUTTONPIN = 4;
#define RELAYPIN = 7;
#define SENSOR1PIN = 8;
#define SENSOR2PIN = 9;
#define RESOLUTION = 0.5;



LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
SimpleDHT11 sensor1(SENSOR1PIN);
SimpleDHT11 sensor2(SENSOR2PIN);


int redButtonState;
int blackButtonState;
int yellowButtonState;
int relayState;
int counter = 0;
float setTemperature = 20;

void setup() {
    lcd.begin(16,2);
    lcd.backlight();   //lcd.noBacklight()

    lcd.setCursor(0,0);
    lcd.print("AIR CONDITIONING");
    lcd.setCursor(5,1);
    lcd.print("CONTROL");
    Serial.begin(9600);
    pinMode(RELAYPIN, OUTPUT);
    pinMode(redButtonPin, OUTPUT);
    pinMode(blackButtonPin, OUTPUT);
    pinMode(yellowButtonPin, OUTPUT);
    delay(5000);
}

void loop() {
    byte temperature1 = 0;
    byte humidity1 = 0;
    byte temperature2 = 0;
    byte humidity2 = 0;
    
    if ((err = sensor1.read(&temperature1, &humidity1, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("Read DHT11 failed, err="); Serial.println(err);delay(1000);
    return;
  }

    if (counter == 3)
        counter = 0;


    redButtonState = digitalRead(redButtonPin);
    blackButtonState = digitalRead(blackButtonPin);
    yellowButtonState = digitalRead(yellowButtonPin);
    relayState = digitalRead(relayPin);
    Serial.print("Relay: ");
    Serial.println(relayState);
    Serial.print("Button: ");
    Serial.println(redButtonState);
    Serial.print("Counter: ");
    Serial.println(counter);

    if (redButtonState == HIGH) {
        counter++;
    }
  
    if (counter == 0){
        lcd.clear();
        lcd.setCursor(5,0);
        lcd.print("23.9");
        lcd.print((char) 223);
        lcd.print("C");
        lcd.setCursor(7,1);
        lcd.print("OFF");
        digitalWrite(relayPin, HIGH);
    }
  
    else if (counter == 1){
        lcd.clear();
        lcd.setCursor(5,0);
        lcd.print("23.9");
        lcd.print((char) 223);
        lcd.print("C");
        lcd.setCursor(7,1);
        lcd.print("ON");
        digitalWrite(relayPin, LOW);
    }
  
    else if (counter == 2){
        lcd.clear();
        lcd.setCursor(5,0);
        lcd.print("23.9");
        lcd.print((char) 223);
        lcd.print("C");
        lcd.setCursor(0,1);
        lcd.print("Automatic: ");
        lcd.print(setTemperature);
        lcd.print((char) 223);
        lcd.print("C");
        
        if (setTemperature < 23.9){
            digitalWrite(relayPin, LOW);
            lcd.setCursor(0,0);
            lcd.print("ON ");
        }
        else if (setTemperature >= 23.9){
            digitalWrite(relayPin, HIGH);
            lcd.setCursor(0,0);
            lcd.print("OFF");
        }
        
        if (blackButtonState == HIGH) {
            setTemperature = setTemperature - RESOLUTION;
        }

        if (yellowButtonState == HIGH) {
            setTemperature = setTemperature + RESOLUTION;
        }
    }
 

    delay(200);

}
