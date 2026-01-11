/*
 * [RU] Приемник беспроводного угломера.
 * Получает данные через LoRa, фильтрует выбросы и выводит на TFT.
 * [EN] Wireless Inclinometer Receiver.
 * Receives LoRa data, performs robust averaging, and displays on TFT.
 */

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include "FontsRus/TimesNRCyr12.h" // Русские шрифты
#include <algorithm> 

#define TFT_CS 5
#define TFT_RST 4
#define TFT_DC 2
#define TFT_BL 15
#define RX1_PIN 16
#define TX1_PIN 17

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

const int numReadings = 10;
float rollReadings[numReadings], pitchReadings[numReadings];
int readingIndex = 0;
float roll = 0.0, pitch = 0.0;

// Robust average (trimmed mean) / Среднее арифметическое с отсечением выбросов
float calculateAverage(float data[], int size) {
    if (size <= 2) return 0;
    float temp[size];
    for (int i = 0; i < size; i++) temp[i] = data[i];
    std::sort(temp, temp + size);
    float sum = 0;
    for (int i = 1; i < size - 1; i++) sum += temp[i];
    return sum / (size - 2);
}

void updateDisplay() {
    // Clear value areas / Очистка областей значений
    tft.fillRect(160, 50, 80, 40, ST77XX_BLACK);
    tft.fillRect(160, 100, 80, 40, ST77XX_BLACK);

    tft.setTextColor(ST77XX_WHITE);
    tft.setCursor(160, 80);
    tft.print(pitch, 1);
    tft.setCursor(160, 130);
    tft.print(roll, 1);
}

void setup() {
    Serial.begin(115200);
    Serial1.begin(115200, SERIAL_8N1, RX1_PIN, TX1_PIN);

    tft.init(240, 320);
    tft.setRotation(3);
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);
    tft.fillScreen(ST77XX_BLACK);

    tft.setFont(&TimesNRCyr12pt8b);
    tft.setCursor(0, 80);
    tft.print("Прицел:"); // Pitch
    tft.setCursor(0, 130);
    tft.print("Крен:");   // Roll
}

void loop() {
    if (Serial1.available()) {
        String dataString = Serial1.readStringUntil('\n');
        int commaIndex = dataString.indexOf(',');

        if (commaIndex != -1) {
            pitchReadings[readingIndex] = dataString.substring(0, commaIndex).toFloat();
            rollReadings[readingIndex] = dataString.substring(commaIndex + 1).toFloat();
            readingIndex = (readingIndex + 1) % numReadings;

            pitch = calculateAverage(pitchReadings, numReadings);
            roll = calculateAverage(rollReadings, numReadings);
            updateDisplay();
        }
    }
    delay(1);
}
