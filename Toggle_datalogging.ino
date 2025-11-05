#include <SPI.h>
#include <SD.h>

#define CS_PIN 5 // Chip select pin for SD card
#define BUTTON_PIN 15  // Change this pin if needed

File logFile;
bool isLogging = false;  // Flag to track if logging is active
bool lastButtonState = HIGH;  // Store the previous button state (HIGH means not pressed with pull-up)

float temperature = 0;
float humidity = 0;
unsigned long timestamp = 0;

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(2, OUTPUT);
  Serial.begin(115200);

  if (!SD.begin(CS_PIN)) {
    Serial.println("SD card initialization failed!");
    return;
  }
  Serial.println("SD card initialized.");

  logFile = SD.open("datalog.csv", FILE_WRITE);
  if (logFile) {
    logFile.println("Timestamp,Temperature (C),Humidity (%)");
    logFile.close();
  } else {
    Serial.println("Failed to open datalog.csv");
  }
}

void handleButtonToggle() {
  bool currentButtonState = digitalRead(BUTTON_PIN);

  if (lastButtonState == HIGH && currentButtonState == LOW) {
    isLogging = !isLogging;  // Toggle logging
    Serial.println(isLogging ? "Logging started." : "Logging stopped.");

    digitalWrite(2, isLogging ? HIGH : LOW); // Turn on LED when logging
    delay(200);  // Debounce delay
  }
  lastButtonState = currentButtonState;
}

void logging() {
  logFile = SD.open("/datalog.csv", FILE_APPEND);
  if (logFile) {
    logFile.print(timestamp);
    logFile.print(",");
    logFile.print(temperature);
    logFile.print(",");
    logFile.println(humidity);
    logFile.close();
  } else {
    Serial.println("Error opening datalog.csv");
  }
}

void loop() {
  handleButtonToggle();

  if (isLogging) {
    temperature = random(200, 350) / 10.0;
    humidity = random(300, 800) / 10.0;
    timestamp = millis();

    Serial.print("Time: ");
    Serial.print(timestamp);
    Serial.print(" ms, Temp: ");
    Serial.print(temperature);
    Serial.print(" C, Humidity: ");
    Serial.println(humidity);

    logging(); // Log data every second
    delay(1000);
  }
}
