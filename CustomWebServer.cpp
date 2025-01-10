#include <sys/_intsup.h>
#include "WString.h"
#include "CustomWebServer.h"

MyWebServer::MyWebServer()
  : server(80) {}

void MyWebServer::begin(WiFiManager* wm) {
  wifiManager = wm;

  server.on("/", HTTP_GET, [this]() {
    handleRoot();
  });
  server.on("/wifi", HTTP_GET, [this]() {
    handleWiFi();
  });
  server.on("/scan", HTTP_GET, [this]() {
    handleScanNetworks();
  });
  server.on("/connect", HTTP_POST, [this]() {
    handleConnect();
  });
  server.on("/clear", HTTP_GET, [this]() {
    handleClearCredentials();
  });

  server.begin();
}

void MyWebServer::serverSend(String html, int errorNumber = 200, boolean homePageBtn = true) {
  String htmlHeader = "<html><meta charset='utf-8'><meta name='viewport' content='width=device-width, initial-scale=1'><body>";
  if (homePageBtn) {
    htmlHeader += "<form method='get' action='/'><button type='submit'>На главную</button></form>";
  }
  String htmlEnd = "</body></html>";
  server.send(errorNumber, "text/html", htmlHeader + html + htmlEnd);
}

void MyWebServer::handleClient() {
  server.handleClient();
}

void MyWebServer::handleRoot() {
  String html = "<a href='/wifi'>Настройки подключения wifi</a>";
  serverSend(html, 200, false);
}

void MyWebServer::handleWiFi() {
  String html = "<h1>Wi-Fi менеджер</h1><form method='get' action='/scan'><button>Сканировать сети</button></form>";
  html += "<form method='get' action='/clear'><button>Удалить сохранённую сеть</button></form>";
  html += "<p>Подключённый IP: " + wifiManager->getConnectedIP() + "</p>";
  serverSend(html);
}

void MyWebServer::handleScanNetworks() {
  String html = "<h1>Выберите Wi-Fi сеть</h1>";
  html += "<form method='post' action='/connect'>";

  // Сканируем сети и создаем радиокнопки для каждой найденной сети
  int n = WiFi.scanNetworks();
  if (n == 0) {
    html += "<p>Не найдено сетей</p>";
  } else {
    for (int i = 0; i < n; ++i) {
      int rssi = WiFi.RSSI(i); // Уровень сигнала сети
      String strength;

      // Оценка уровня сигнала (добавляем индикатор)
      if (rssi >= -50) {
        strength = "Очень сильный сигнал";
      } else if (rssi >= -70) {
        strength = "Хороший сигнал";
      } else if (rssi >= -85) {
        strength = "Средний сигнал";
      } else {
        strength = "Слабый сигнал";
      }

      // Выводим радио-кнопку и уровень сигнала
      html += "<label><input type='radio' name='ssid' value='" + WiFi.SSID(i) + "'>";
      html += WiFi.SSID(i) + " (" + strength + ", " + String(rssi) + " dBm)</label><br>";
    }
  }

  html += "<br><label>Пароль: <input type='password' name='password'></label><br><br>";
  html += "<button type='submit'>Подключиться</button>";
  html += "</form>";
  serverSend(html);
}


void MyWebServer::handleConnect() {
  if (server.hasArg("ssid")) {  // Проверка наличия аргументов
    String ssid = server.arg("ssid");
    String password = server.hasArg("password") ? server.arg("password") : "";  // Получаем пароль

    // Пытаемся подключиться к Wi-Fi
    wifiManager->connectToNetwork(ssid.c_str(), password.c_str());

    // Проверяем статус подключения
    if (wifiManager->isConnected()) {
      // Успешное подключение
      String html = "<h1>Подключение успешно!</h1>";
      html += "<p>Вы подключены к сети: <strong>" + ssid + "</strong></p>";
      html += "<p>Ваш IP-адрес: " + wifiManager->getConnectedIP() + "</p>";
      html += "<form method='get' action='/'><button>На главную</button></form>";
      serverSend(html, 200, false);
    } else {
      // Ошибка подключения
      String html = "<h1>Ошибка подключения</h1>";
      html += "<p>Не удалось подключиться к сети: <strong>" + ssid + "</strong></p>";
      html += "<p>Проверьте правильность введенного пароля и попробуйте снова.</p>";
      html += "<form method='get' action='/wifi'><button>Попробовать снова</button></form>";
      serverSend(html, 400, false);
    }
  } else {
    // Если SSID не указан
    String html = "<h1>Ошибка</h1>";
    html += "<p>Вы не выбрали сеть для подключения.</p>";
    html += "<form method='get' action='/scan'><button>Вернуться к списку сетей</button></form>";
    serverSend(html, 400, false);
  }
}


void MyWebServer::handleClearCredentials() {
  wifiManager->clearCredentials();
  String html = "<p>Сохранённая сеть удалена.</p>";
  serverSend(html);
}
