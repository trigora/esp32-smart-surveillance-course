#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h> // Библиотека для MQTT
#include "config.h"       // Наши секретные данные

#include <WiFiClientSecure.h> // Шаг 1: Заменяем/добавляем библиотеку для SSL
WiFiClientSecure wifiClient; // Шаг 2: Меняем тип клиента на защищённый

// Создаём объекты для сети и MQTT
// WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

// Функция для подключения к Wi-Fi
void connectToWiFi() {
  Serial.print("Подключение к Wi-Fi ");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" Готово!");
  Serial.print("IP-адрес: ");
  Serial.println(WiFi.localIP());
}

// Функция для подключения к MQTT-брокеру
void connectToMQTT() {
  Serial.print("Подключение к MQTT-брокеру...");

  // КРИТИЧЕСКИ ВАЖНО для порта 9992 (SSL):
  wifiClient.setInsecure();

  mqttClient.setServer(MQTT_BROKER, MQTT_PORT);
  // Используем MQTT_CLIENT_ID из config.h!
  if (mqttClient.connect(MQTT_CLIENT_ID, MQTT_USER, MQTT_PASS)) {
    Serial.println(" Готово!");
  } else {
    Serial.print(" Ошибка, код: ");
    Serial.println(mqttClient.state());
    delay(5000);
    // Здесь лучше не вызывать функцию рекурсивно, а просто выйти
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000); // Даём время на инициализацию

  connectToWiFi();    // Шаг 1: Подключаемся к Wi-Fi
  connectToMQTT();    // Шаг 2: Подключаемся к облачному брокеру
}

void loop() {
  // Обязательно поддерживаем соединение с MQTT
  if (!mqttClient.connected()) {
    connectToMQTT();
  }
  mqttClient.loop();

  // Каждые 10 секунд публикуем тестовое сообщение
  static unsigned long lastMsgTime = 0;
  if (millis() - lastMsgTime > 10000) {
    lastMsgTime = millis();

    // Формируем полное имя топика: префикс + "text"
    String fullTopic = String(MQTT_TOPIC_PREFIX) + "text";
    String message = "Привет от ESP32! Время: " + String(millis() / 1000) + "с";

    // Публикуем
    if (mqttClient.publish(fullTopic.c_str(), message.c_str())) {
      Serial.println("Сообщение опубликовано в топик: " + fullTopic);
    } else {
      Serial.println("Ошибка публикации!");
    }
  }
  delay(100); // Небольшая задержка для стабильности
}