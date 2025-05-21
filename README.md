# Air Quality Monitoring System using ESP32, MQ-135, MQ-2, MQ-5, LCD, and Buzzer

## 📌 Project Overview

This project is an **Air Quality Monitoring System** built using an **ESP32 microcontroller** and **gas sensors (MQ-135, MQ-2, MQ-5)** to detect harmful gases in the environment. It displays the readings on an LCD screen and triggers a buzzer when pollutant levels exceed a safe threshold.

The system is ideal for monitoring air quality in homes, schools, industries, or urban environments.

---

## 🔧 Features

- Real-time monitoring of air pollutants using:
  - **MQ-135**: Measures air quality (ammonia, alcohol, smoke, benzene)
  - **MQ-2**: Detects LPG, smoke, methane, propane
  - **MQ-5**: Detects LPG, natural gas, and town gas
- Displays sensor readings on a **16x2 LCD** screen
- **Buzzer alert** when gas levels cross safety thresholds
- Compact, low-power, and portable
- Can be extended for IoT/cloud integration

---

## 🧰 Components Required

| Component        | Quantity |
|------------------|----------|
| ESP32 Dev Board  | 1        |
| MQ-135 Gas Sensor| 1        |
| MQ-2 Gas Sensor  | 1        |
| MQ-5 Gas Sensor  | 1        |
| 16x2 LCD (I2C)   | 1        |
| Buzzer           | 1        |
| Breadboard + Wires | As needed |
| 5V Power Supply  | 1        |

---

## 🔌 Circuit Connections

### Gas Sensors
| Sensor | VCC | GND | Analog Out |
|--------|-----|-----|-------------|
| MQ-135 | 3.3V/5V | GND | A0 |
| MQ-2   | 3.3V/5V | GND | A1 |
| MQ-5   | 3.3V/5V | GND | A2 |

*Connect the analog outputs of sensors to ESP32’s analog input pins (e.g., GPIO34, GPIO35, GPIO32).*

### LCD (with I2C)
| LCD Pin | ESP32 Pin |
|---------|-----------|
| VCC     | 3.3V or 5V|
| GND     | GND       |
| SDA     | GPIO21    |
| SCL     | GPIO22    |

### Buzzer
| Buzzer Pin | ESP32 Pin |
|------------|-----------|
| + (VCC)    | GPIO25    |
| - (GND)    | GND       |

---

## 💻 Code Overview

- Reads analog values from MQ sensors
- Converts readings into gas concentrations (approximation)
- Displays live data on LCD
- Buzzer rings if any sensor value crosses threshold

---

## 🧪 Example Thresholds

These can be adjusted based on calibration:
```cpp
#define MQ135_THRESHOLD 300
#define MQ2_THRESHOLD   400
#define MQ5_THRESHOLD   350
