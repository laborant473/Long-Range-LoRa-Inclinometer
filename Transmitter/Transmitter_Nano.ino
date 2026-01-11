/*
 * [RU] Передатчик беспроводного угломера. 
 * Считывает данные с MPU6500 через FastIMU, фильтрует и отправляет через LoRa.
 * [EN] Wireless Inclinometer Transmitter.
 * Reads MPU6500 via FastIMU, filters data, and sends via LoRa UART.
 */

#include <Wire.h>
#include "F_MPU6500.hpp" // Библиотека FastIMU. library for MPU6500. ...FastIMU\src\F_MPU6500.hpp

#define GREEN_LED_PIN 3 
#define RED_LED_PIN   2 

MPU6500 mpu;
const float alpha = 0.8; // Complementary filter coefficient / Коэффициент фильтра
float roll = 0, pitch = 0;
unsigned long lastTime = 0;

void setup() {
    Serial.begin(115200); // Connects to LoRa E220 UART
    Wire.begin();

    calData calibration;
    // Default bias values / Значения смещения по умолчанию
    for(int i=0; i<3; i++) { calibration.accelBias[i] = 0; calibration.gyroBias[i] = 0; }

    int initResult = mpu.init(calibration, 0x68);
    if (initResult == 0) {
        // Success: LED Blinking / Успех: Индикация светодиодами
        for (int i = 0; i < 5; i++) {
            digitalWrite(GREEN_LED_PIN, HIGH); digitalWrite(RED_LED_PIN, HIGH);
            delay(100);
            digitalWrite(GREEN_LED_PIN, LOW); digitalWrite(RED_LED_PIN, LOW);
            delay(100);
        }

        digitalWrite(RED_LED_PIN, HIGH); // Calibration start / Начало калибровки
        mpu.calibrateAccelGyro(&calibration);
        calibration.valid = true;

        digitalWrite(GREEN_LED_PIN, HIGH);
        digitalWrite(RED_LED_PIN, LOW);
        lastTime = millis();
    } else {
        // Error / Ошибка инициализации
        digitalWrite(RED_LED_PIN, HIGH);
        while (true);
    }
}

void loop() {
    mpu.update();
    AccelData accel;
    GyroData gyro;
    mpu.getAccel(&accel);
    mpu.getGyro(&gyro);

    // Accel angles / Углы по акселерометру
    float accRoll = atan2(accel.accelY, accel.accelZ) * 180.0 / PI;
    float accPitch = atan2(-accel.accelX, sqrt(accel.accelY * accel.accelY + accel.accelZ * accel.accelZ)) * 180.0 / PI;

    // Time delta / Расчет времени шага
    unsigned long currentTime = millis();
    float dt = (float)(currentTime - lastTime) / 1000.0f;
    lastTime = currentTime;

    // Complementary Filter / Комплементарный фильтр
    roll = alpha * (roll + gyro.gyroX * dt) + (1.0 - alpha) * accRoll;
    pitch = alpha * (pitch + gyro.gyroY * dt) + (1.0 - alpha) * accPitch;

    // Data output: pitch,roll / Вывод данных: тангаж,крен
    Serial.print(pitch, 2);
    Serial.print(",");
    Serial.println(roll, 2);

    delay(100);
}
