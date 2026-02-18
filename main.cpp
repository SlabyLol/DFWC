#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

WebServer server(80);

bool limitedMode = false;
String selectedSSID = "";
String selectedPassword = "";
bool wifiConnected = false;

void scanNetworksHTML(String &html) {
  int n = WiFi.scanNetworks();
  if (n == 0) {
    html += "<p>No networks found</p>";
  } else {
    for (int i = 0; i < n; ++i) {
      html += "<form action='/connect' method='POST'>";
      html += "<input type='hidden' name='ssid' value='" + WiFi.SSID(i) + "'>";
      html += "<b>" + WiFi.SSID(i) + "</b> ";
      html += "<input type='password' name='password' placeholder='Password'>";
      html += "<input type='submit' value='Connect'>";
      html += "</form><hr>";
    }
  }
}

void handleRoot() {
  String html = "<h1>DFWC Setup</h1>";

  if (!wifiConnected && !limitedMode) {
    html += "<h2>Select WiFi</h2>";
    scanNetworksHTML(html);
    html += "<br><a href='/skip'>Skip (Limited Mode)</a>";
  } else {
    html += "<h2>System Running</h2>";
    if (limitedMode) {
      html += "<p>LIMITED MODE ACTIVE</p>";
    } else {
      html += "<p>Connected to: " + selectedSSID + "</p>";
    }
  }

  server.send(200, "text/html", html);
}

void handleConnect() {
  selectedSSID = server.arg("ssid");
  selectedPassword = server.arg("password");

  WiFi.mode(WIFI_STA);
  WiFi.begin(selectedSSID.c_str(), selectedPassword.c_str());

  Serial.println("Connecting to WiFi...");

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    wifiConnected = true;
    Serial.println("\nConnected!");
    Serial.println(WiFi.localIP());
  }

  server.sendHeader("Location", "/");
  server.send(302, "text/plain", "");
}

void handleSkip() {
  limitedMode = true;
  server.sendHeader("Location", "/");
  server.send(302, "text/plain", "");
}

void startAccessPoint() {
  WiFi.softAP("JustATest", "T1E2S3T4");
  Serial.println("Hotspot started");
  Serial.println(WiFi.softAPIP());
}

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_AP_STA);
  startAccessPoint();

  server.on("/", handleRoot);
  server.on("/connect", HTTP_POST, handleConnect);
  server.on("/skip", handleSkip);

  server.begin();
}

void loop() {
  server.handleClient();
}
