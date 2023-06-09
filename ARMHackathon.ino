// Include necessary libraries
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <RTClib.h>

// Define constant values for pins and sensor types
#define DHTTYPE DHT22
#define DHTPIN 2
#define SOILPIN A0
#define LDRPIN A1
#define BUZZERPIN 3
#define LEDPIN 13
#define I2C_ADDR 0x27

// Initialize LiquidCrystal_I2C and DHT objects, and RTC_DS1307 object
LiquidCrystal_I2C lcd(I2C_ADDR,16,2);
DHT dht(DHTPIN, DHTTYPE);
RTC_DS1307 rtc;

// Function to display current time on LCD and Serial monitor
void displayTime() {
  DateTime now = rtc.now();
  lcd.setCursor(0, 0);
  lcd.print("Time: ");
  lcd.print(now.hour(), DEC);
  lcd.print(':');
  lcd.print(now.minute(), DEC);
  lcd.print(':');
  lcd.print(now.second(), DEC);

  Serial.print("\nTime: ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC); 
}

// Function to display current temperature and humidity on LCD and Serial monitor
void displayTempHum() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  
  // Check if sensor reading is valid
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  
  // Display temperature and humidity on LCD
  lcd.setCursor(0, 1);
  lcd.print("Temp: ");
  lcd.print(t);
  lcd.print((char)223);
  lcd.print("C");
  lcd.print(" Humidity: ");
  lcd.print(h);
  lcd.print("%");
  
  // Scroll the text to the left on LCD display
  for (int i = 0; i < 16; i++) {
    lcd.scrollDisplayLeft();
    delay(200);
  }
 
  // Display temperature and humidity on Serial monitor
  Serial.print("\nTemp: ");
  Serial.print(t);
  Serial.print((char)223);
  Serial.print("C");
  Serial.print(" Humidity: ");
  Serial.print(h);
  Serial.print("%");
}

// Function to display current soil moisture level on LCD and Serial monitor
void displaySoilMoisture() {
  lcd.clear();
  int soilMoisture = analogRead(SOILPIN);
  lcd.setCursor(0, 2);
  lcd.print("Soil Moisture:");
  lcd.print(soilMoisture);

  Serial.print("\nSoil Moisture:");
  Serial.print(soilMoisture);
}

// Function to check temperature level and trigger LED and buzzer if it is above threshold
void checkTemp() {
  float t = dht.readTemperature();
  if (t >= 30) {
    digitalWrite(LEDPIN, HIGH);
    tone(BUZZERPIN, 1000, 1000);
  } else {
    digitalWrite(LEDPIN, LOW);
    noTone(BUZZERPIN);
  }
}

// Function to check humidity level and trigger LED and buzzer if it is below threshold
void checkHumidity() {
  float h = dht.readHumidity();
  if (h <= 30) {
    digitalWrite(LEDPIN, HIGH);
    tone(BUZZERPIN, 1000, 1000);
 

  } else {
    digitalWrite(LEDPIN, LOW);
    noTone(BUZZERPIN);
  }
}
void checkSoilMoisture() {
  int soilMoisture = analogRead(SOILPIN);
  if (soilMoisture < 300) {
    digitalWrite(LEDPIN, HIGH);
    tone(BUZZERPIN, 1000, 1000);
  } else {
    digitalWrite(LEDPIN, LOW);
    noTone(BUZZERPIN);
  }
}
void checkLight() {
  int lightLevel = analogRead(LDRPIN);
  if (lightLevel > 100) {
    digitalWrite(LEDPIN, HIGH);
    tone(BUZZERPIN, 1000, 1000);
  } else {
    digitalWrite(LEDPIN, LOW);
    noTone(BUZZERPIN);
  }
}
void displayLight() {
  int lightLevel = analogRead(LDRPIN);
  lcd.setCursor(0, 3);
  lcd.print("Light Level: ");
  lcd.print(lightLevel);

  Serial.print("\nLight Level: ");
  Serial.print(lightLevel);
}
 

void setup() {
  pinMode(LEDPIN, OUTPUT);
  pinMode(BUZZERPIN, OUTPUT);
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Initializing...");
  delay(2000);
  dht.begin();
  Wire.begin();
  rtc.begin();
if (! rtc.isrunning()) {
    lcd.setCursor(0, 0);
  lcd.print("RTC is NOT running!");
  rtc.adjust(DateTime(__DATE__, __TIME__));
  }
  lcd.clear();
  lcd.print("System ready!");
}
void loop() {
  delay(2000);
  lcd.clear();
  displayTime();
  delay(1000);
  
  checkTemp();
  checkHumidity();
  displayTempHum();
  
  checkSoilMoisture();
  displaySoilMoisture();
  
  checkLight();
  displayLight();
  
  
  delay(2000);
  displayTempHum();
  delay(2000);
  
  delay(2000);
 
  delay(2000);
  lcd.clear();
  delay(1000);
}
