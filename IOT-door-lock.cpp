#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "ssid";
const char* password = "password";

const int motorPin1 = 4; // Motor driver input pin 1
const int motorPin2 = 5; // Motor driver input pin 2

WebServer server(80);

void setup() {
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);

  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, handleRoot);
  server.on("/on", HTTP_GET, handleFanOn);
  server.on("/off", HTTP_GET, handleFanOff);
  server.begin();
}

void loop() {
  server.handleClient();
}

void handleRoot() {
  String html = "<html><body>";
  html += "<h1>Fan Control</h1>";
  html += "<p>Fan is currently: ";
  html += (digitalRead(motorPin1) == HIGH) ? "On" : "Off";
  html += "</p>";
  html += "<a href=\"/on\"><button>Turn Off</button></a>";
  html += "<a href=\"/off\"><button>Turn On</button></a>";
  html += "</body></html>";

  server.send(200, "text/html", html);
}

void handleFanOn() {
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  server.sendHeader("Location", "/");
  server.send(302, "text/plain", "");
}

void handleFanOff() {
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  server.sendHeader("Location", "/");
  server.send(302, "text/plain", "");
}
