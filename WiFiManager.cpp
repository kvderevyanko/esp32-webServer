#include "WiFiManager.h"
#include <Preferences.h>

Preferences preferences;

WiFiManager::WiFiManager() : isConnectedFlag(false) {}

void WiFiManager::begin() {
    WiFi.mode(WIFI_AP_STA); // Одновременная работа в режиме точки доступа и клиента
    WiFi.softAP("ESP32_APP", "12345678"); // Настройки точки доступа

    // Установка статического IP-адреса для точки доступа
    IPAddress local_IP(192, 168, 5, 1);      // IP-адрес
    IPAddress gateway(192, 168, 5, 1);       // Шлюз
    IPAddress subnet(255, 255, 255, 0);      // Маска подсети
    WiFi.softAPConfig(local_IP, gateway, subnet);

    // Загружаем сохраненные данные
    loadCredentials();
    
    if (!savedSSID.isEmpty()) {
        connectToNetwork(savedSSID.c_str(), savedPassword.c_str());
    }
}

void WiFiManager::update() {
    if (WiFi.status() != WL_CONNECTED && !savedSSID.isEmpty()) {
        connectToNetwork(savedSSID.c_str(), savedPassword.c_str());
    }
}

void WiFiManager::connectToNetwork(const char* ssid, const char* password) {
    WiFi.begin(ssid, password);

    Serial.print("Подключение к сети: ");
    Serial.println(ssid);

    unsigned long startAttemptTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000) {
        delay(500);
        Serial.print(".");
    }
    
    isConnectedFlag = (WiFi.status() == WL_CONNECTED);
    
    if (isConnectedFlag) {
        Serial.println("\nПодключено!");
        saveCredentials(ssid, password);
    } else {
        Serial.println("\nНе удалось подключиться.");
    }
}

void WiFiManager::saveCredentials(const char* ssid, const char* password) {
    preferences.begin("wifi", false);
    preferences.putString("ssid", ssid);
    preferences.putString("password", password);
    preferences.end();
}

void WiFiManager::loadCredentials() {
    preferences.begin("wifi", true);
    savedSSID = preferences.getString("ssid", "");
    savedPassword = preferences.getString("password", "");
    preferences.end();
}

void WiFiManager::clearCredentials() {
    preferences.begin("wifi", false);
    preferences.clear();
    preferences.end();
    savedSSID = "";
    savedPassword = "";
}

String WiFiManager::getConnectedIP() {
    if (isConnectedFlag) {
        return WiFi.localIP().toString();
    }
    return "Not connected";
}

bool WiFiManager::isConnected() const {
    return isConnectedFlag;
}

String WiFiManager::getSavedSSID() const {
    return savedSSID;
}

String WiFiManager::getSavedPassword() const {
    return savedPassword;
}
