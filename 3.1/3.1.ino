#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>

// WiFi credentials
char ssid[] = "2.4G";
char pass[] = "WASD3.1415926";

// IFTTT Webhooks key (from your IFTTT account)
String key = "blQRbxPzlbKNssDhYfV2oM";

// Sensor configuration
const int lightSensorPin = A0;
int threshold = 500;
bool light = false;

// IFTTT server address
char server[] = "maker.ifttt.com";

// Create WiFi and HTTP client instances
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
  Serial.println("\nConnected to WiFi!");
}

void loop() {
  // Read analog value from the light sensor
  int lightValue = analogRead(lightSensorPin);
  Serial.print("Light Level: ");
  Serial.println(lightValue);

  // Detect if light appears and hasn't already been detected
  if (!light && lightValue > threshold) {
    light = true;
    sendIFTTT("light_detected"); // Send event to IFTTT when light is detected
  }
  // Detect if light disappears after being detected
  else if (light && lightValue < threshold - 100) {
    light = false;
    sendIFTTT("light_end");
  }

  delay(5000);
}

// Function to send an event to IFTTT using Webhooks
void sendIFTTT(String eventName) {
  String url = "/trigger/" + eventName + "/with/key/" + key;
  client.get(url);

  int statusCode = client.responseStatusCode();
  String response = client.responseBody();

  Serial.print("IFTTT Response Code: ");
  Serial.println(statusCode);
  Serial.println(response);
}