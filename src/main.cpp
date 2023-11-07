#include <WiFi.h>
#include <HTTPClient.h>
#include <OneWire.h>
#include <PubSubClient.h>
#include "header.h"

// Relays for switching appliances
#define Relay1 2

int switch_ON_Flag1_previous_I = 0;
const unsigned long interval = 1000; // Vertraging van 1 seconde

// Subscribed Topics
#define sub1 "output"

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

int DS18S20_Pin = D2; // DS18S20 Signal pin on digital 2
float temperatureLimit = 35.0; // Temperature limit in Celsius
const unsigned long deepSleepInterval = 60; // Diepe slaap na 60 seconden

OneWire ds(DS18S20_Pin); // on digital pin 2

// in deze functie gaat de code verbinding maken met de wifi
void setup_wifi() {
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    // Wacht tot de verbinding is gemaakt of een time-out heeft
    unsigned long currentMillis = millis();
    if (currentMillis - lastMsg >= interval) {
      lastMsg = currentMillis;
      Serial.print(".");
    }
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

// zorgt voor de werking via MQTT 
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  if (strstr(topic, sub1)) {
    for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
    }
    Serial.println();

    if ((char)payload[0] == 'f') {
      digitalWrite(Relay1, LOW);
    } else {
      digitalWrite(Relay1, HIGH);
    }
  } else {
    Serial.println("Unsubscribed topic");
  }
}

// in deze functie gaat de ESP32 verbining maken met de raspberry pi
void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);

    if (client.connect(clientId.c_str(), username, pass)) {
      Serial.println("Connected");
      client.publish("outTopic", "Hello, world");
      client.subscribe(sub1);
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" Try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  pinMode(Relay1, OUTPUT);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1884);
  client.setCallback(callback);
}

// in deze functie gaat de melding verstuurd worden naar telegram
void sendTelegramMessage(String message) {
  HTTPClient http;

  String url = "https://api.telegram.org/bot" + String(botToken) + "/sendMessage?chat_id=" + String(chatId) + "&text=" + message;

  http.begin(url);

  int httpCode = http.GET();

  if (httpCode > 0) {
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println("Message sent to Telegram: " + message);
    }
  } else {
    Serial.println("Error sending message to Telegram");
  }

  http.end();
}

float getTemp() {
  byte data[12];
  byte addr[8];

  if (!ds.search(addr)) {
    ds.reset_search();
    return -1000;
  }

  if (OneWire::crc8(addr, 7) != addr[7]) {
    Serial.println("CRC is not valid!");
    return -1000;
  }

  if (addr[0] != 0x10 && addr[0] != 0x28) {
    Serial.print("Device is not recognized");
    return -1000;
  }

  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);

  byte present = ds.reset();
  ds.select(addr);
  ds.write(0xBE);

  for (int i = 0; i < 9; i++) {
    data[i] = ds.read();
  }

  ds.reset_search();

  byte MSB = data[1];
  byte LSB = data[0];

  float tempRead = ((MSB << 8) | LSB);
  float TemperatureSum = tempRead / 16;

  return TemperatureSum;
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long currentMillis = millis();
  if (currentMillis - lastMsg >= interval) {
    lastMsg = currentMillis;

    float temperature = getTemp();
    Serial.println(temperature);

    if (temperature > temperatureLimit) {
      Serial.println("Gevaar: Limiet overschreden");
      sendTelegramMessage("Gevaar: Limiet overschreden: " + String(temperature) + " °C");
    } else {
      // Als de temperatuur onder de limiet komt, ga in diepe slaap
      esp_sleep_enable_timer_wakeup(deepSleepInterval * 1000000); // omrekenen naar microseconden
      esp_deep_sleep_start();
    }
  }
}
