#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

#define IR_ENTRY 2
#define IR_EXIT 3
#define PIR_SENSOR 4
#define LED 5
#define BUZZER 6
#define SERVO_PIN 9

const int MAX_CAPACITY = 5;
int carCount = 0;

Servo gateServo;

void setup() {
  pinMode(IR_ENTRY, INPUT);
  pinMode(IR_EXIT, INPUT);
  pinMode(PIR_SENSOR, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  gateServo.attach(SERVO_PIN);
  gateServo.write(0); 

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print(" YOGITA_Techno ");
  lcd.setCursor(0, 1);
  lcd.print("Cars: 0");

  Serial.begin(9600);
}

void loop() {
  if (digitalRead(IR_ENTRY) == LOW) {
    if (carCount < MAX_CAPACITY) {
      carCount++;
      updateLCD();
      openGate();
      delay(1000);
    } else {
      lcd.setCursor(0, 1);
      lcd.print(" Parking Full! ");
      digitalWrite(BUZZER, HIGH);
      delay(2000);
      digitalWrite(BUZZER, LOW);
      updateLCD();
    }
  }

  if (digitalRead(IR_EXIT) == LOW) {
    if (carCount > 0) {
      carCount--;
      updateLCD();
      openGate();
      delay(1000);
    }
  }

  if (digitalRead(PIR_SENSOR) == HIGH) {
    digitalWrite(LED, HIGH);
    digitalWrite(BUZZER, HIGH);
    delay(1000);
  } else {
    digitalWrite(LED, LOW);
    digitalWrite(BUZZER, LOW);
  }

  delay(100);
}

void openGate() {
  gateServo.write(90); // Open
  delay(2000); // Wait for car to pass
  gateServo.write(0);  // Close
}

void updateLCD() {
  lcd.setCursor(0, 1);
  lcd.print("Cars: ");
  lcd.print(carCount);
  lcd.print("   ");
}
