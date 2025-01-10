#include <Arduino.h>
#include "WiFiManager.h"
#include "CustomWebServer.h"

WiFiManager wifiManager;
MyWebServer webServer;

unsigned long lastWiFiUpdate = 0;          // Переменная для отслеживания времени последнего обновления Wi-Fi
const unsigned long wifiUpdateInterval = 1000; // Интервал обновления Wi-Fi (1 секунда)

void setup() {
    Serial.begin(115200);

    // Инициализация Wi-Fi менеджера
    Serial.println("Инициализация Wi-Fi...");
    wifiManager.begin();

    // Печать состояния подключения
    if (wifiManager.isConnected()) {
        Serial.print("Подключено к сети. IP-адрес: ");
        Serial.println(wifiManager.getConnectedIP());
    } else {
        Serial.println("Устройство работает в режиме точки доступа.");
    }

    // Инициализация веб-сервера
    Serial.println("Запуск веб-сервера...");
    webServer.begin(&wifiManager);
}

void loop() {
    // Обновляем статус Wi-Fi через заданный интервал времени
    unsigned long currentMillis = millis();
    if (currentMillis - lastWiFiUpdate >= wifiUpdateInterval) {
        wifiManager.update();
        lastWiFiUpdate = currentMillis;
    }

    // Обрабатываем клиентские запросы к веб-серверу
    webServer.handleClient();
}
