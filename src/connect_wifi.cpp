#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <FS.h>

AsyncWebServer server(80);
/*
void onIndexRequest(AsyncWebServerRequest *request);
void onCSSRequest(AsyncWebServerRequest *request);
void onJSRequest(AsyncWebServerRequest *request);
void onWsJsRequest(AsyncWebServerRequest *request);
void onBGSvgRequest(AsyncWebServerRequest *request);
void onFavIconRequest(AsyncWebServerRequest *request);
void onPageNotFound(AsyncWebServerRequest *request);
*/
/*
// Callback: send homepage
void onIndexRequest(AsyncWebServerRequest *request)
{
    IPAddress remote_ip = request->client()->remoteIP();
    Serial.println("[" + remote_ip.toString() +
                   "] HTTP GET request of " + request->url());
    request->send(SPIFFS, "/index.html", "text/html");
}

// Callback: send style sheet
void onCSSRequest(AsyncWebServerRequest *request)
{
    IPAddress remote_ip = request->client()->remoteIP();
    Serial.println("[" + remote_ip.toString() +
                   "] HTTP GET request of " + request->url());
    request->send(SPIFFS, "/style-alt.css", "text/css");
}

// Callback: send javascript
void onJSRequest(AsyncWebServerRequest *request)
{
    IPAddress remote_ip = request->client()->remoteIP();
    Serial.println("[" + remote_ip.toString() +
                   "] HTTP GET request of " + request->url());
    request->send(SPIFFS, "/script.js", "text/javascript");
}

// Callback: send web socket javascript
void onWsJsRequest(AsyncWebServerRequest *request)
{
    IPAddress remote_ip = request->client()->remoteIP();
    Serial.println("[" + remote_ip.toString() +
                   "] HTTP GET request of " + request->url());
    request->send(SPIFFS, "/script_ws.js", "text/javascript");
}

// Callback: send favicon
void onFavIconRequest(AsyncWebServerRequest *request)
{
    IPAddress remote_ip = request->client()->remoteIP();
    Serial.println("[" + remote_ip.toString() +
                   "] HTTP GET request of " + request->url());
    request->send(SPIFFS, "/icon.png", "image/png");
}

// Callback: send image bg.svg
void onBGSvgRequest(AsyncWebServerRequest *request)
{
    IPAddress remote_ip = request->client()->remoteIP();
    Serial.println("[" + remote_ip.toString() +
                   "] HTTP GET request of " + request->url());
    request->send(SPIFFS, "/bg.svg", "image/svg+xml");
}

// Callback: send 404 if requested file does not exist
void onPageNotFound(AsyncWebServerRequest *request)
{
    IPAddress remote_ip = request->client()->remoteIP();
    Serial.println("[" + remote_ip.toString() +
                   "] HTTP GET request of " + request->url());
    request->send(404, "text/plain", "Not found");
}
*/
//testing WiFi connection
bool testWifi(const char *w_ssid, const char *w_passwd)
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(w_ssid, w_passwd);
    int c = 0;
    Serial.println("Waiting for Wifi to connect");
    while (c < 20)
    {
        if (WiFi.status() == WL_CONNECTED)
        {
            return true;
        }
        delay(500);
        Serial.print(".");
        c++;
    }
    Serial.println("");
    Serial.println("Connect timed out, opening AP");
    return false;
}

//create the web server
void createWebServer()
{
    /*
    // On HTTP request for root, provide index.html file
    server.on("/", HTTP_GET, onIndexRequest);

    // On HTTP request for style sheet, provide style.css
    server.on("/style-alt.css", HTTP_GET, onCSSRequest);

    // On HTTP request for javascript, provide script.js
    server.on("/script.js", HTTP_GET, onJSRequest);

    // On HTTP request for javascript, provide script_ws.js
    server.on("/script_ws.js", HTTP_GET, onWsJsRequest);

    // On HTTP request for image, provide bg.svg
    server.on("/bg.svg", HTTP_GET, onBGSvgRequest);

    // On HTTP request for favicon, provide icon.png
    server.on("/favicon.ico", HTTP_GET, onFavIconRequest);

    // Handle requests for pages that do not exist
    server.onNotFound(onPageNotFound);
    */
    // On HTTP request for root, provide index.html file
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        IPAddress remote_ip = request->client()->remoteIP();
        Serial.println("[" + remote_ip.toString() +
                       "] HTTP GET request of " + request->url());
        request->send(SPIFFS, "/index.html", "text/html");
    });

    // On HTTP request for style sheet, provide style.css
    server.on("/style-alt.css", HTTP_GET, [](AsyncWebServerRequest *request) {
        IPAddress remote_ip = request->client()->remoteIP();
        Serial.println("[" + remote_ip.toString() +
                       "] HTTP GET request of " + request->url());
        request->send(SPIFFS, "/style-alt.css", "text/css");
    });

    // On HTTP request for javascript, provide script.js
    server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        IPAddress remote_ip = request->client()->remoteIP();
        Serial.println("[" + remote_ip.toString() +
                       "] HTTP GET request of " + request->url());
        request->send(SPIFFS, "/script.js", "text/javascript");
    });

    // On HTTP request for javascript, provide script_ws.js
    server.on("/script_ws.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        IPAddress remote_ip = request->client()->remoteIP();
        Serial.println("[" + remote_ip.toString() +
                       "] HTTP GET request of " + request->url());
        request->send(SPIFFS, "/script_ws.js", "text/javascript");
    });

    // On HTTP request for image, provide bg.svg
    server.on("/bg.svg", HTTP_GET, [](AsyncWebServerRequest *request) {
        IPAddress remote_ip = request->client()->remoteIP();
        Serial.println("[" + remote_ip.toString() +
                       "] HTTP GET request of " + request->url());
        request->send(SPIFFS, "/bg.svg", "image/svg+xml");
    });

    // On HTTP request for image, provide refresh.svg
    server.on("/refresh.svg", HTTP_GET, [](AsyncWebServerRequest *request) {
        IPAddress remote_ip = request->client()->remoteIP();
        Serial.println("[" + remote_ip.toString() +
                       "] HTTP GET request of " + request->url());
        request->send(SPIFFS, "/refresh.svg", "image/svg+xml");
    });

    // On HTTP request for image, provide eye-close.svg
    server.on("/eye-close.svg", HTTP_GET, [](AsyncWebServerRequest *request) {
        IPAddress remote_ip = request->client()->remoteIP();
        Serial.println("[" + remote_ip.toString() +
                       "] HTTP GET request of " + request->url());
        request->send(SPIFFS, "/eye-close.svg", "image/svg+xml");
    });

    // On HTTP request for image, provide eye-up.svg
    server.on("/eye-up.svg", HTTP_GET, [](AsyncWebServerRequest *request) {
        IPAddress remote_ip = request->client()->remoteIP();
        Serial.println("[" + remote_ip.toString() +
                       "] HTTP GET request of " + request->url());
        request->send(SPIFFS, "/eye-up.svg", "image/svg+xml");
    });

    // On HTTP request for favicon, provide icon.png
    server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request) {
        IPAddress remote_ip = request->client()->remoteIP();
        Serial.println("[" + remote_ip.toString() +
                       "] HTTP GET request of " + request->url());
        request->send(SPIFFS, "/icon.png", "image/png");
    });

    // Handle requests for pages that do not exist
    server.onNotFound([](AsyncWebServerRequest *request) {
        IPAddress remote_ip = request->client()->remoteIP();
        Serial.println("[" + remote_ip.toString() +
                       "] HTTP GET request of " + request->url());
        request->send(404, "text/plain", "Not found");
    });
}

//starting server
void launchWeb()
{
    Serial.println("");
    if (WiFi.status() == WL_CONNECTED)
        Serial.println("WiFi connected");
    Serial.print("Local IP: ");
    Serial.println(WiFi.localIP());
    Serial.print("SoftAP IP: ");
    Serial.println(WiFi.softAPIP());
    createWebServer();
    // Start the server
    server.begin();
    Serial.println("Server started");
}

//starting WiFi soft AP
void startAP()
{
    WiFi.disconnect();
    delay(100);
    WiFi.softAP("AGTech_Dev", "");
    Serial.println("Initializing softAP");
    launchWeb();
}