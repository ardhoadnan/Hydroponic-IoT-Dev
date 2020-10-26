#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <WebSocketsServer.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <DHT.h>
#include <FS.h>

//definisi pin I2C
#define SDA_PIN D2
#define SCL_PIN D1

//definisi sensor DHT
#define DHTPIN D4     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11 // DHT 11
DHT dht(DHTPIN, DHTTYPE);

//wifi default name
const char *ssid = "";
const char *password = "";
WiFiClient wifiClient; //definisi wificlient untuk mqttclient

//definisi server
//AsyncWebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(1337);

//nama client mqtt dari chip id
String mqttClientId = String("ESP8266Client-") + ESP.getChipId();

//IPAddress mqttServer(192, 168, 99, 102);
PubSubClient mqttClient(wifiClient);

//definisi mqtt setting
int mqttPort;
String mqttServer;
String mqttUsername;

int t_interval = 2;  //untuk doPublish
bool active = false; //untuk json

File configFile; //spiffs config json

int light;          //nilai sensor cahaya
String stringLight; //string yang diterima dari sensor cahaya I2C

//include function
bool doPublish(int i_on);
bool testWifi(const char *w_ssid, const char *w_passwd);
void startAP();

// Callback: receiving any WebSocket message
void onWebSocketEvent(uint8_t client_num,
                      WStype_t type,
                      uint8_t *payload,
                      size_t length)
{

  // Figure out the type of WebSocket event
  switch (type)
  {

  // Client has disconnected
  case WStype_DISCONNECTED:
    Serial.printf("[%u] Disconnected!\n", client_num);
    break;

  // New client has connected
  case WStype_CONNECTED:
  {
    IPAddress ip = webSocket.remoteIP(client_num);
    Serial.printf("[%u] Connection from ", client_num);
    Serial.println(ip.toString());
  }
  break;

  // Handle text messages from client
  case WStype_TEXT:

    // Print out raw message
    Serial.printf("[%u] Received text: %s\n", client_num, payload);

    /*// Toggle LED
      if ( strcmp((char *)payload, "toggleLED") == 0 ) {
        led_state = led_state ? 0 : 1;
        Serial.printf("Toggling LED to %u\n", led_state);
        digitalWrite(led_pin, led_state);

      // Report the state of the LED
      } else if ( strcmp((char *)payload, "getLEDState") == 0 ) {
        sprintf(msg_buf, "%d", led_state);
        Serial.printf("Sending to [%u]: %s\n", client_num, msg_buf);
        webSocket.sendTXT(client_num, msg_buf);

      // Message not recognized
      } else {
        Serial.println("[%u] Message not recognized");
      }*/
    break;

  // For everything else: do nothing
  case WStype_BIN:
  case WStype_ERROR:
  case WStype_FRAGMENT_TEXT_START:
  case WStype_FRAGMENT_BIN_START:
  case WStype_FRAGMENT:
  case WStype_FRAGMENT_FIN:
  default:
    break;
  }
}

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

void doWsBroadcast()
{
  int w = WiFi.scanNetworks();
  if (w == 0)
    Serial.println("No networks found.");
  else
  {
    StaticJsonDocument<500> arraySSID;
    JsonArray ssid_array = arraySSID.to<JsonArray>();
    for (int i = 0; i < w; ++i)
    {
      ssid_array.add(WiFi.SSID(i));
    }
    char jBuffer[500];                 //json buffer
    serializeJson(arraySSID, jBuffer); //membuat json array format dalam char buffer
    webSocket.broadcastTXT(jBuffer);
    //Serial.println(jBuffer);
  }
}

void setup()
{
  Wire.begin(SDA_PIN, SCL_PIN);
  Serial.begin(115200);
  SPIFFS.begin();
  delay(500);

  //buka file config
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
      ssid = jsonDoc["wifi_ssid"].as<char *>();
      password = jsonDoc["wifi_password"].as<char *>();
      mqttServer = jsonDoc["mqtt_server"].as<char *>();
      mqttUsername = jsonDoc["mqtt_token"].as<char *>();
      mqttPort = jsonDoc["mqtt_port"];
    }
  }
  configFile.close();

  //trying to connect wifi
  WiFi.disconnect(); //disconnect all wifi connection
  if (testWifi(ssid, password))
  {
    Serial.println("Connected bro");
    return;
  }
  else
  {
    Serial.println("Turn on Hotspot mode.");
    startAP();
  }

  // Start WebSocket server and assign callback
  webSocket.begin();
  webSocket.onEvent(onWebSocketEvent);

  Serial.println();
  Serial.print("Tunggu setting selesai ");

  int milDelay = 0;
  long currentMillis;
  while (WiFi.status() != WL_CONNECTED)
  {
    // Look for and handle WebSocket data
    webSocket.loop();
    if (currentMillis != millis())
    {
      milDelay++;
      currentMillis = millis();
    }
    if (milDelay == 500) //delay 
    {
      //Serial.print("start at ");
      //Serial.println(millis());
      // send data to all connected clients
      doWsBroadcast(); //ada delay 2s 100ms dari scanNetworks
      //Serial.print("sent at ");
      //Serial.println(millis());
      //Serial.print(".");
      milDelay = 0;
    }
  }
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
    //if (mqttClient.connect(mqttClientId.c_str(), mqttUsername.c_str(), NULL))
    if (mqttClient.connect(mqttClientId.c_str()))
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
  Wire.requestFrom(8, 4); //request from slave device (target device 8)
  stringLight = "";
  while (Wire.available())
  {
    //light = Wire.read();
    char c = Wire.read();
    stringLight = stringLight + c;
    //light = (charLight);
  }
  light = stringLight.toInt();
  //Serial.print("light (I2C): ");
  //Serial.println(stringLight);

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
  jsonSensor["light"] = light;
  jsonSensor["active"] = active;
  char jBuffer[500];                  //buffer variable sensor
  serializeJson(jsonSensor, jBuffer); //membuat json format dalam char buffer

  //Serial.println("qq");

  if (doPublish(t_interval) == true)
  {
    mqttClient.publish("v1/devices/me/attributes", aBuffer);
    mqttClient.publish("v1/devices/me/telemetry", jBuffer);
    Serial.println("Data sent.");

    Serial.print("light (I2C): ");
    Serial.println(light);
  }
  delay(100);
}
