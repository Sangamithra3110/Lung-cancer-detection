#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h> // Include EEPROM library

/*void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}*/


 

// Create an LCD object with I2C address 0x27 (commonly used for many LCD modules)
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Set the LCD address and dimensions (16 columns, 2 rows)

// Define the analog pin connected to the MQ-135 sensor
const int mq135Pin = A0;

// Variables to store the raw sensor value and the CO2 concentration level
int sensorValue = 0;  // Raw analog sensor value
int co2Level = 0;     // Mapped CO2 level in parts per million (ppm)

// Define a reasonable range for CO2 levels (adjust based on calibration)
const int minCo2Level = 350;  // Minimum expected CO2 level in ppm
const int maxCo2Level = 1000; // Maximum expected CO2 level in ppm

// Function to clear the EEPROM
void clearEEPROM() {
  for (int i = 0; i < EEPROM.length(); i++) {
    EEPROM.write(i, 0);  // Clear EEPROM by writing 0 to each byte
  }
  Serial.println("EEPROM cleared.");
}

void setup() {
  // Initialize the LCD and Serial Monitor
  lcd.begin(16, 2);
  lcd.setBacklight(true);  // Turn on the backlight
  Serial.begin(9600);

  // Display an initial message
  lcd.setCursor(0, 0);
  lcd.print("Lung Health Monitor");
  delay(2000);  // Display for 2 seconds
  lcd.clear();  // Clear the LCD

  // Clear EEPROM (optional, remove if unnecessary)
  clearEEPROM();
}

void loop() {
  // Read the analog value from the MQ-135 sensor
  sensorValue = analogRead(mq135Pin);

  // Map the raw sensor value to CO2 level in ppm (adjust range based on calibration)
  co2Level = map(sensorValue, 0, 1023, 0, 1000);

  // Display an error if the CO2 level is out of range
  if (co2Level < minCo2Level || co2Level > maxCo2Level) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Error: Check Sensor");
    Serial.println("Error: Sensor reading out of range.");
  } else {
    // Display the CO2 level on the LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("CO2 Level: ");
    lcd.setCursor(0, 1);
    lcd.print(co2Level);
    lcd.print(" ppm");

    // Print the CO2 level to the Serial Monitor
    Serial.print("CO2 Level: ");
    Serial.print(co2Level);
    Serial.println(" ppm");

    // Store the CO2 level in EEPROM (store only if different to reduce writes)
    if (EEPROM.read(0) != co2Level) {
      EEPROM.write(0, co2Level);  
    }
  }

  // Wait for 1 second before updating
  delay(1000);
}
