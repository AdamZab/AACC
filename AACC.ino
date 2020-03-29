#include <Wire.h>  
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address, if it's not working try 0x27.

const int relayPin = 7;
const int redButtonPin = 4;
const int blackButtonPin = 3;
const int yellowButtonPin = 2;

int redButtonState;
int blackButtonState;
int yellowButtonState;
int relayState;
int counter = 0;
float setTemperature = 20;

void setup() {
  lcd.begin(16,2);   // iInit the LCD for 16 chars 2 lines
  lcd.backlight();   // Turn on the backligt (try lcd.noBaklight() to turn it off)

  lcd.setCursor(0,0); //First line
  lcd.print("AIR CONDITIONING");
  lcd.setCursor(5,1); //Second line
  lcd.print("CONTROL");
  Serial.begin(9600);
  pinMode(relayPin, OUTPUT);
  pinMode(redButtonPin, OUTPUT);
  pinMode(blackButtonPin, OUTPUT);
  pinMode(yellowButtonPin, OUTPUT);
   delay(4000);
}

void loop() {

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
    setTemperature = setTemperature - 0.5;
    }

    if (yellowButtonState == HIGH) {
      setTemperature = setTemperature + 0.5;
    }
  }
 

delay(200);

}
