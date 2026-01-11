# RU. Wireless Long-Range Inclinometer (LoRa)

Система дистанционного мониторинга углов наклона и крена, работающая на дистанции до **11.8 км**. 
Разработано для контроля положения наземных робототехнических комплексов (UGV) и мониторинга состояния удаленных инженерных сооружений.

## 📡 Характеристики связи
- **Радиомодули:** LoRa E220-400T30D (433 МГц, мощность до 1 Вт).
- **Дальность:** 11 800 метров (подтверждено тестами).
- **Антенны:** Самодельные Ground Plane, КСВ < 1.15.
- **Высота установки:** 2 метра над уровнем земли.

## 🛠 Архитектура системы

### Передатчик (Сенсорный блок)
- **Контроллер:** Arduino Nano v3.
- **Датчик:** MPU6500 (IMU, 6-осевой гироскоп/акселерометр).
- **Связь:** Передача данных в прозрачном режиме через LoRa E220-400T30D (30dBm мощность).
- **Интерфейс: UART.

### Приемник (Базовая станция)
- **Контроллер:** ESP32 Dev Module.
- **Дисплей:** TFT Display (ST7789 или аналог).
- **Связь:** Прием данных через LoRa E220-400T30D (чувствительность приёма —  −147dBm).
- **Интерфейс:** Обработка входящих пакетов и визуализация углов Pitch/Roll в реальном времени.

## 📡 Особенности антенного хозяйства
Особое внимание в проекте уделено качеству связи. Использование всенаправленных антенн типа "Ground Plane",
настроенных вручную, позволило достичь стабильной передачи данных на дистанции более 11 км 
при минимальной высоте подвеса.

##  "Обработка данных" (Software Processing):
1.  На передатчике: 
    Для обеспечения стабильности показаний в условиях вибраций (актуально для робототехники) реализован **комплементарный фильтр**. 
    - Объединяет данные акселерометра и гироскопа.
    - Коэффициент фильтрации $\alpha = 0.96$.
    - Устраняет высокочастотные шумы акселерометра и низкочастотный дрейф гироскопа.

2.  На приемнике: 
    *   Буферизация входящих пакетов.
    *   Алгоритм Trimmed Mean (усеченное среднее): автоматическое отсечение минимальных и максимальных значений 
    в серии измерений. Это гарантирует отсутствие резких «скачков» значений на дисплее при единичных ошибках 
    в радиоканале.

## 🔧 Применение
1. **Робототехника:** Телеметрия кренов мобильных платформ. Мониторинг телеметрии наземных беспилотников (UGV).
2. **Авионика:** Дополнительный горизонт для БПЛА.
3. **Строительство:** Контроль крена высотных конструкций, мачт и опор в том числе в труднодоступных местах.
4. **Сельское хозяйство:** Контроль положения спецтехники на сложных рельефах.

## 📝 Лицензия
Этот проект распространяется под лицензией MIT.


# EN. Wireless Long-Range Inclinometer (LoRa)

A remote monitoring system for pitch and roll angles, operating at distances up to 11.8 km.
Developed for attitude control of Unmanned Ground Vehicles (UGV) and structural health monitoring of remote engineering facilities.

## 📡 Communication Specifications
- Radio Modules: LoRa E220-400T30D (433 MHz, up to 1W power).
- Range: 11,800 meters (field-verified).
- Antennas: DIY Ground Plane antennas, VSWR < 1.15.
- Installation Height: 2 meters above ground level.

## 🛠 System Architecture

### Transmitter (Sensor Node)
- MCU: Arduino Nano v3.
- Sensor: MPU6500 (6-axis IMU: gyroscope and accelerometer).
- Communication: Transparent data transmission via LoRa E220-400T30D (30dBm output power).
- Interface: UART.

### Receiver (Base Station)
- MCU: ESP32 Dev Module.
- Display: TFT Display (ST7789 or compatible).
- Communication: Data reception via LoRa E220-400T30D (receiving sensitivity down to -147dBm).
- Interface: Real-time packet processing and visualization of Pitch/Roll angles.

## 📡 Antenna System Features
The project places special emphasis on communication quality. The use of manually tuned omnidirectional "Ground Plane" antennas allowed for a stable data link over 11 km, even at a minimum installation height.

## 💻 Software Processing

### 1. Transmitter Side:
To ensure stable readings under vibration (critical for robotics), a Complementary Filter is implemented:
- Fuses data from the accelerometer and gyroscope.
- Filter coefficient α = 0.96.
- Eliminates high-frequency accelerometer noise and low-frequency gyroscope drift.

### 2. Receiver Side:
- Packet Buffering: Smooths out the incoming data stream.
- Trimmed Mean Algorithm: Automatically rejects the minimum and maximum values in a measurement series. This ensures the absence of sudden "jumps" or spikes on the display caused by occasional radio interference or bit errors.

## 🔧 Applications
1. Robotics: Attitude telemetry for mobile platforms and Unmanned Ground Vehicles (UGV).
2. Avionics: Auxiliary artificial horizon for UAVs.
3. Construction: Tilt monitoring for high-rise structures, masts, and towers, including those in hard-to-reach locations.
4. Agriculture: Slope and attitude monitoring for specialized machinery on complex terrains.

## 📝 License
This project is distributed under the MIT License.