#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include <WiFi.h>
#include <Preferences.h> // Для работы с сохранением данных

class WiFiManager {
public:
    WiFiManager();
    
    // Инициализация Wi-Fi (точка доступа и клиент)
    void begin();
    
    // Обновление состояния Wi-Fi (для повторного подключения при потере сигнала)
    void update();
    
    // Подключение к сети Wi-Fi
    void connectToNetwork(const char* ssid, const char* password);
    
    // Сохранение учетных данных
    void saveCredentials(const char* ssid, const char* password);
    
    // Загрузка сохраненных учетных данных
    void loadCredentials();
    
    // Очистка сохраненных учетных данных
    void clearCredentials();
    
    // Получение IP-адреса при подключении
    String getConnectedIP();
    
    // Получение статуса подключения
    bool isConnected() const;
    
    // Геттеры для SSID и пароля
    String getSavedSSID() const;
    String getSavedPassword() const;

private:
    String savedSSID;        // Сохраненный SSID
    String savedPassword;    // Сохраненный пароль
    bool isConnectedFlag;    // Флаг статуса подключения
    
    Preferences preferences; // Для работы с сохраненными данными
};

#endif
