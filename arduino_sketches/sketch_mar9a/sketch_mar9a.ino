#include <stdio.h>
#include <stdlib.h>

int dutyCycle = 50;
int byteCount = 0;
// int bytes[3] = {0};
char byteRead = 0;
int pinSelected = 13;
int pinOther = 12;

void setup() {
  Serial.begin(9600);
  // IN1
  pinMode(12, OUTPUT);
  digitalWrite(12, HIGH);

  // PWM the opposite pin...
  analogWrite(13, 100 - dutyCycle);
}

void loop() {
  dutyCycle = 0;

  while ((byteRead = Serial.read()) != '\n') {
    if (byteRead != -1) {
      Serial.print("Byte read: ");
      Serial.println(byteRead, DEC);
      dutyCycle = dutyCycle*10 + byteRead - 48;
      Serial.print("Dutycycle: ");
      Serial.println(dutyCycle);
      Serial.print("\n");
    }
  }
  
  if (dutyCycle == 0) {
    if (pinSelected == 13) {
      pinSelected = 12;
      pinOther = 13;
    } else {
      pinSelected = 13;
      pinOther = 12;
    }

  } 

  digitalWrite(pinOther, HIGH);
  analogWrite(pinSelected, 255 - dutyCycle*255/100);
  delay(1000);  
}
