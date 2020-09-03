#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <JC_Button.h>
#include <PubSubClient.h>
#include <FS.h>
#include <ArduinoJson.h>
#include <DHT.h>

#define BUTTON_PIN D2
#define PULLUP true
#define INVERT true
#define DEBOUNCE_MS 20

String mqttClientId = String("ESP8266Client-") + ESP.getChipId();

unsigned long pressedAtMillis;
unsigned long const interval = 5000;
unsigned long pressedForMillis;

const char *ssid = "ZIAN  on hotspot";
const char *password = "khanif12345";

int mqttPort;
String mqttServer;
String mqttUsername;

#define DHTPIN D4     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11 // DHT 11
DHT dht(DHTPIN, DHTTYPE);

int t_interval = 2;
bool active = false;

//IPAddress mqttServer(192, 168, 99, 102);
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);
char msg[50];

File configFile;

bool doPublish(int i_on);

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String text = "";
  for (int i = 0; i < length; i++)
  {
    //Serial.print((char) payload[i]);
    text += (char)payload[i];
  }
  Serial.println(text);

  delay(4000);
}

void setup()
{
  Serial.begin(115200);

  delay(1000);

  SPIFFS.begin();
  configFile = SPIFFS.open("/config.json", "r");

  if (!configFile)
  {
    Serial.println("/config.json file doesn't exist!");
  }
  else
  {
    Serial.println("Using a /config.json settings file");
    size_t fileSize = configFile.size();

    uint8_t buf[fileSize];
    configFile.read(buf, fileSize);
    Serial.println((char *)buf);
    StaticJsonDocument<256> jsonDoc;
    DeserializationError parseError = deserializeJson(jsonDoc, (char *)buf);
    if (parseError)
    {
      Serial.println("Failed to parse config");
      delay(1000);
      return;
    }
    else
    {
      mqttServer = jsonDoc["mqtt_server"].as<char*>();
      mqttUsername = jsonDoc["mqtt_token"].as<char*>();
      mqttPort = jsonDoc["mqtt_port"];
    }
  }

  configFile.close();
  mqttClient.setCallback(callback);
  dht.begin();
}

void reconnect()
{
  mqttClient.setServer(mqttServer.c_str(), mqttPort);
  // Loop until we're reconnected
  while (!mqttClient.connected())
  {
    Serial.print("Attempting MQTT connection to ");
    Serial.print(mqttServer.c_str());

    // Attempt to connect
    if (mqttClient.connect(mqttClientId.c_str(), mqttUsername.c_str(), NULL))
    {
      Serial.print(mqttClientId.c_str());
      Serial.println(" has been connected.");
    }
    else
    {
      Serial.print(" failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void connectWiFi()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    return;
  }

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  unsigned int cnt = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
    if (cnt++ == 30)
    {
      // wipe wifi info after 30 attempts
      // wifi ap may not be available anymore
      Serial.println("Can't connect, resetting WiFi info");
      WiFi.disconnect();
      ESP.restart();
      delay(3000);
    }
  }
  Serial.println("Connected");

  Serial.println(String("My IP: ") + WiFi.localIP().toString());
  delay(3000);
}

void loop()
{
  /*
  btn.read();

  if (btn.wasPressed())
  {
    pressedAtMillis = millis();
  }

  if (btn.wasReleased())
  {
    if (pressedForMillis > interval)
    {
      // we have a long press, reset
      Serial.println("Resetting WiFi credentials");
      WiFi.disconnect();

      delay(3000);
      ESP.restart();
    }
    else
    {
      // short press, turn on the display
      delay(3000);
      // somewhere around mid-screen with this font and size
      Serial.println(String("My IP: ") + WiFi.localIP().toString());
      Serial.println(String("MQTT Connected: ") + mqttClient.connected());
      if (mqttClient.connected())
      {
        Serial.println(String("Subscriber ID: ") + mqttClientId);
      }
      delay(5000);
      Serial.println("Going to deep sleep");
      Serial.println("Going to deep sleep");
      delay(3000);
      // 5 minutes deep sleep
      ESP.deepSleep(60 * 5 * 1000 * 1000, RF_DEFAULT);
      delay(3000); // this will not really get executed
    }
  }

  pressedForMillis = millis() - pressedAtMillis;
  */
  connectWiFi();

  if (!mqttClient.connected())
  {
    reconnect();
  }
  mqttClient.loop();

  //json untuk attribute
  StaticJsonDocument<500> jsonAttribute;
  jsonAttribute["firmware_version"] = "1.0.1";
  jsonAttribute["serial_number"] = "SN-001";
  char aBuffer[500];                           //buffer variable attribute
  serializeJsonPretty(jsonAttribute, aBuffer); //membuat json format dalam char buffer

  //json untuk sensor
  StaticJsonDocument<500> jsonSensor;
  jsonSensor["temperature"] = dht.readTemperature();
  jsonSensor["humidity"] = dht.readHumidity();
  jsonSensor["active"] = active;
  char jBuffer[500];                        //buffer variable sensor
  serializeJsonPretty(jsonSensor, jBuffer); //membuat json format dalam char buffer

  if (doPublish(t_interval))
  {
    mqttClient.publish("v1/devices/me/attributes", aBuffer);
    mqttClient.publish("v1/devices/me/telemetry", jBuffer);
    Serial.println("Data sent.");
  }
}
