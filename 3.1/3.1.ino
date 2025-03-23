#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>

char ssid[] = "2.4G";
char pass[] = "WASD3.1415926";

const int lightSensorPin = A0;
int threshold = 500;
bool light = false;

char server[] = "maker.ifttt.com";
String eventOn = "light_detected";
String key = "/blQRbxPzlbKNssDhYfV2oM";

WiFiClient wifi;
HttpClient client = HttpClient(wifi, server, 80);

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.print("Connecting to WiFi");
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\nConnected!");
}

void loop() {
  int lightValue = analogRead(lightSensorPin);
  Serial.print("Light Level: ");
  Serial.println(lightValue);

  if (!light && lightValue > threshold) {
    light = true;
    sendIFTTT(eventOn);
  } 
  else if (light && lightValue < threshold - 100) {
    light = false;
    sendIFTTT("light_ended");
  }

  delay(5000);
}

void sendIFTTT(String eventName) {
  String url = "/trigger/" + eventName + "/with/key/" + key;
  client.get(url);

  int statusCode = client.responseStatusCode();
  String response = client.responseBody();

  Serial.print("IFTTT Response: ");
  Serial.println(statusCode);
  Serial.println(response);
}