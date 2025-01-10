#ifndef MYWEBSERVER_H
#define MYWEBSERVER_H

#include <WiFi.h>
#include <WebServer.h>  // Подключаем встроенный WebServer для ESP32
#include "WiFiManager.h"

class MyWebServer {
public:
    MyWebServer();
    
    // Инициализация веб-сервера
    void begin(WiFiManager* wifiManager);
    
    // Обработка клиентских запросов
    void handleClient();

private:
    WebServer server;  // Используем библиотечный WebServer для работы с HTTP
    WiFiManager* wifiManager;

    // Обработчики для различных маршрутов
    void handleRoot();
    void serverSend(String html, int errorNumber, boolean homePageBtn);
    void handleWiFi();
    void handleScanNetworks();
    void handleConnect();
    void handleClearCredentials();
};

#endif
