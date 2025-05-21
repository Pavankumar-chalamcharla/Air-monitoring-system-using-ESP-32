#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <math.h>

// LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// I2C Pins for ESP32
#define I2C_SDA 21
#define I2C_SCL 22

// MQ Sensor Pins
#define MQ2_PIN     35
#define MQ135_PIN   32

// Load Resistance in kOhms
#define RL          10.0

// Calibrated R0 values in clean air (Example only; calibrate yours!)
#define MQ2_R0      9.83
#define MQ135_R0    10.0

// DHT11 Sensor
#define DHTPIN      33
#define DHTTYPE     DHT11
DHT dht(DHTPIN, DHTTYPE);

// Buzzer
#define BUZZER_PIN  25

// Log curve constants for CO and NH3
#define A_CO    -0.77
#define B_CO     1.699
#define A_NH3   -0.47
#define B_NH3    1.698

// --- Function to calculate PPM with correction ---
float calculatePPM(int adc, float R0, float a, float b, float correctionFactor) {
  float RS = ((4095.0 - adc) / adc) * RL;
  RS /= correctionFactor; // Apply temp correction
  float ratio = RS / R0;
  float ppm = pow(10, (a * log10(ratio) + b));
  return ppm;
}

// --- AQI category from CO ppm ---
String getAQICategory(float co_ppm) {
  if (co_ppm <= 4.4) return "Good";
  else if (co_ppm <= 9.4) return "Moderate";
  else if (co_ppm <= 12.4) return "UnhlthySG";
  else if (co_ppm <= 15.4) return "Unhealthy";
  else if (co_ppm <= 30.4) return "VeryBad";
  else return "Hazardous";
}

// --- Temperature correction factor (adjust for your environment) ---
float getCorrectionFactor(float temp, float hum) {
  float factor = 1.0;

  if (temp < 20) factor = 1.1;
  else if (temp > 30) factor = 0.9;

  return factor;
}

void setup() {
  Serial.begin(9600);
  Wire.begin(I2C_SDA, I2C_SCL);
  lcd.init();
  lcd.backlight();
  pinMode(BUZZER_PIN, OUTPUT);
  dht.begin();

  lcd.setCursor(0, 0);
  lcd.print("Air Quality Mon");
  delay(1000);
  lcd.clear();
}

void loop() {
  int mq2_adc = analogRead(MQ2_PIN);
  int mq135_adc = analogRead(MQ135_PIN);

  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  if (isnan(temp) || isnan(hum)) {
    temp = hum = 0;
  }

  float correction = getCorrectionFactor(temp, hum);

  // Calculate corrected PPM
  float co_ppm = calculatePPM(mq135_adc, MQ135_R0, A_CO, B_CO, correction);
  float nh3_ppm = calculatePPM(mq2_adc, MQ2_R0, A_NH3, B_NH3, correction);

  // Get AQI from CO
  String aqi = getAQICategory(co_ppm);

  // --- LCD: CO & NH3 ---
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("CO: ");
  lcd.print(co_ppm, 1);
  lcd.print("ppm");
  lcd.setCursor(0, 1);
  lcd.print("NH3: ");
  lcd.print(nh3_ppm, 1);
  lcd.print("ppm");
  delay(2000);

  // --- LCD: Temp & Hum ---
  lcd.clear();
  delay(1000);

  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temp);
  lcd.print("C");
  lcd.setCursor(0, 1);
  lcd.print("Hum: ");
  lcd.print(hum);
  lcd.print(" %");
  delay(2000);
lcd.clear();
  delay(1000);
  // --- LCD: AQI display ---
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("AQI: ");
  lcd.print(aqi);
  lcd.setCursor(0, 1);
  lcd.print("CO: ");
  lcd.print(co_ppm, 1);
  lcd.print("ppm");
  delay(2000);
  lcd.clear();
  delay(1000);

  // --- Serial debug ---
  Serial.print("CO: "); Serial.print(co_ppm); Serial.print(" ppm");
  Serial.print(" | NH3: "); Serial.print(nh3_ppm); Serial.print(" ppm");
  Serial.print(" | Temp: "); Serial.print(temp); Serial.print(" C");
  Serial.print(" | Hum: "); Serial.print(hum); Serial.print(" %");
  Serial.print(" | AQI: "); Serial.println(aqi);

  // --- Buzzer alert ---
  if (aqi == "Unhealthy" || aqi == "VeryBad" || aqi == "Hazardous") {
    digitalWrite(BUZZER_PIN, HIGH);
  } else {
    digitalWrite(BUZZER_PIN, LOW);
  }
}