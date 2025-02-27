#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// Initialize LCD (Check I2C address with scanner)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Define IR sensor pins
#define ENTRY_SENSOR 2
#define EXIT_SENSOR 3

// Define servo motor pin
#define SERVO_PIN 9

Servo gateServo;
int totalSlots = 5;  
int availableSlots = 5;

void setup() {
    pinMode(ENTRY_SENSOR, INPUT);
    pinMode(EXIT_SENSOR, INPUT);
    gateServo.attach(SERVO_PIN);
    gateServo.write(0);  // Ensure closed position
    delay(500);

    lcd.begin(16, 2);
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Parking Slots: ");
    lcd.setCursor(0, 1);
    lcd.print(availableSlots);
}

void loop() {
    // Entry Detection
    if (digitalRead(ENTRY_SENSOR) == HIGH && availableSlots > 0) {  
        delay(500);  
        availableSlots--;
        updateLCD();

        // Open gate
        gateServo.write(90);
        delay(2000);  
        gateServo.write(0);
    }

    // If parking is full
    if (availableSlots == 0) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Parking Full");
    }

    // Exit Detection
    if (digitalRead(EXIT_SENSOR) == HIGH) {  
        delay(500);
        if (availableSlots < totalSlots) {
            availableSlots++;
            updateLCD();
        }
    }
}

// Function to update LCD display without flickering
void updateLCD() {
    lcd.setCursor(0, 1);
    lcd.print("     ");  // Clear only the number area
    lcd.setCursor(0, 1);
    lcd.print(availableSlots);
}
