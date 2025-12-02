// src/config_sample.h - ШАБЛОН. Скопируйте его в config.h и заполните.
#ifndef CONFIG_H
#define CONFIG_H

// Настройки Wi-Fi
const char* WIFI_SSID = "ВАШ_SSID_РОУТЕРА";
const char* WIFI_PASS = "ВАШ_ПАРОЛЬ_РОУТЕРА";

// Настройки MQTT (получите в личном кабинете ClusterFly)
const char* MQTT_BROKER = "ваш_хост"; // Например, "mqtt1.clusterfly.ru"
const int   MQTT_PORT = 1883; // Или 8883 для SSL
const char* MQTT_USER = "ваш_логин";
const char* MQTT_PASS = "ваш_пароль";
const char* MQTT_TOPIC_PREFIX = "/ваш_префикс/"; // Например, "/user_123456/"

#endif