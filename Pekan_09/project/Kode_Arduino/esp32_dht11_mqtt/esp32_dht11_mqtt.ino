/*
 * ESP32 + DHT11 -> MQTT (broker.hivemq.com)
 *
 * Membaca suhu & kelembaban dari DHT11 dan mengirimkannya ke broker MQTT
 * publik HiveMQ setiap 10 detik.
 *
 * Library yang dibutuhkan (Library Manager):
 *   - "PubSubClient"        oleh Nick O'Leary
 *   - "DHT sensor library"  oleh Adafruit
 *   - "Adafruit Unified Sensor" oleh Adafruit
 *
 * Board : "ESP32 Dev Module" (paket board esp32 by Espressif)
 *
 * Wiring DHT11:
 *   VCC  -> 3V3
 *   DATA -> GPIO 4 (beri pull-up 10k ke 3V3 jika modul tanpa PCB)
 *   GND  -> GND
 *
 * Topic yang dipublish:
 *   <TOPIC_BASE>/suhu    -> mis. 28.50
 *   <TOPIC_BASE>/hum     -> mis. 65.00
 *   <TOPIC_BASE>/data    -> JSON {"suhu":28.5,"kelembaban":65.0}
 */

#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

// ===================== KONFIGURASI =====================
const char* WIFI_SSID     = "NAMA_WIFI_ANDA";
const char* WIFI_PASSWORD = "PASSWORD_WIFI_ANDA";

const char* MQTT_BROKER = "broker.hivemq.com";
const uint16_t MQTT_PORT = 1883;

// Broker publik: topic dapat dibaca siapa saja, ganti "namaanda" agar unik!
const char* TOPIC_BASE = "politani/trk/iot/namaanda/dht11";

#define DHT_PIN   4
#define DHT_TYPE  DHT11

const unsigned long PUBLISH_INTERVAL_MS = 10000;  // 10 detik
// =======================================================

DHT dht(DHT_PIN, DHT_TYPE);
WiFiClient espClient;
PubSubClient mqtt(espClient);

unsigned long lastPublish = 0;
String clientId;

void connectWiFi() {
  if (WiFi.status() == WL_CONNECTED) return;

  Serial.printf("Menghubungkan ke WiFi '%s'", WIFI_SSID);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (millis() - start > 20000) {  // timeout 20 detik -> coba lagi
      Serial.println("\nGagal terhubung, mencoba ulang...");
      WiFi.disconnect();
      WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
      start = millis();
    }
  }
  Serial.printf("\nWiFi terhubung. IP: %s\n", WiFi.localIP().toString().c_str());
}

void connectMQTT() {
  while (!mqtt.connected()) {
    Serial.printf("Menghubungkan ke MQTT %s ... ", MQTT_BROKER);
    if (mqtt.connect(clientId.c_str())) {
      Serial.println("terhubung.");
    } else {
      Serial.printf("gagal (rc=%d), coba lagi dalam 5 detik\n", mqtt.state());
      delay(5000);
      connectWiFi();  // pastikan WiFi masih hidup
    }
  }
}

void publishSensor() {
  float hum  = dht.readHumidity();
  float temp = dht.readTemperature();  // Celsius

  if (isnan(hum) || isnan(temp)) {
    Serial.println("Gagal membaca sensor DHT11, data tidak dikirim.");
    return;
  }

  char sTemp[8], sHum[8], json[64];
  dtostrf(temp, 1, 2, sTemp);
  dtostrf(hum,  1, 2, sHum);
  snprintf(json, sizeof(json), "{\"suhu\":%s,\"kelembaban\":%s}", sTemp, sHum);

  String tTemp = String(TOPIC_BASE) + "/suhu";
  String tHum  = String(TOPIC_BASE) + "/hum";
  String tJson = String(TOPIC_BASE) + "/data";

  bool ok = mqtt.publish(tTemp.c_str(), sTemp);
  ok &= mqtt.publish(tHum.c_str(), sHum);
  ok &= mqtt.publish(tJson.c_str(), json);

  Serial.printf("Suhu: %s C | Kelembaban: %s %% -> publish %s\n",
                sTemp, sHum, ok ? "OK" : "GAGAL");
}

void setup() {
  Serial.begin(115200);
  delay(500);

  dht.begin();

  // Client ID unik berdasarkan MAC agar tidak bentrok di broker publik
  uint64_t mac = ESP.getEfuseMac();
  clientId = "esp32-dht11-" + String((uint32_t)(mac >> 16), HEX);

  connectWiFi();
  mqtt.setServer(MQTT_BROKER, MQTT_PORT);

  // Kirim data pertama segera setelah boot
  lastPublish = millis() - PUBLISH_INTERVAL_MS;
}

void loop() {
  connectWiFi();
  if (!mqtt.connected()) connectMQTT();
  mqtt.loop();

  unsigned long now = millis();
  if (now - lastPublish >= PUBLISH_INTERVAL_MS) {
    lastPublish = now;
    publishSensor();
  }
}
