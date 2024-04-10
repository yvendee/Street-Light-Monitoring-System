#include <WiFi.h>
#include <HTTPClient.h>

#define NUM_LEDS 5
#define LDRPIN 13
#define LEDPIN_1 15
#define LEDPIN_2 16
#define LEDPIN_3 17
#define LEDPIN_4 18
#define LEDPIN_5 19

const char* ssid = "your_wifi_ssid";
const char* password = "your_wifi_password";
const char* serverAddress = "mergelink.000webhostapp.com";
const int serverPort = 80;
const char* endpoint = "/post-sensordata-all.php";

enum LEDStatus {
  OFF = 0,
  ON = 1
};

LEDStatus ledStatus[NUM_LEDS] = {OFF, OFF, OFF, OFF, OFF};

void setup() {
  Serial.begin(115200);
  pinMode(LEDPIN_1, OUTPUT);
  pinMode(LEDPIN_2, OUTPUT);
  pinMode(LEDPIN_3, OUTPUT);
  pinMode(LEDPIN_4, OUTPUT);
  pinMode(LEDPIN_5, OUTPUT);
  pinMode(LDRPIN, INPUT);
  delay(1000);
  connectToWiFi();
}

void loop() {
  int sensorValue = analogRead(LDRPIN);
  Serial.print("LDR Value: ");
  Serial.println(sensorValue);

  if (sensorValue > 500) {
    for (int i = 0; i < NUM_LEDS; i++) {
      ledStatus[i] = ON;
    }
    Serial.println("Low light detected. LEDs turned on.");
  } else {
    for (int i = 0; i < NUM_LEDS; i++) {
      ledStatus[i] = OFF;
    }
    Serial.println("Sunlight detected. LEDs turned off.");
  }

  sendSensorData(ledStatus);

  delay(5000); // Adjust delay as needed
}

void connectToWiFi() {
  Serial.print("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("WiFi connected");
}

void sendSensorData(LEDStatus statuses[]) {
  HTTPClient http;

  // Prepare data
  String postData = "statusforpostnum1=" + String(statuses[0] == ON ? "ON" : "OFF") +
                    "&statusforpostnum2=" + String(statuses[1] == ON ? "ON" : "OFF") +
                    "&statusforpostnum3=" + String(statuses[2] == ON ? "ON" : "OFF") +
                    "&statusforpostnum4=" + String(statuses[3] == ON ? "ON" : "OFF") +
                    "&statusforpostnum5=" + String(statuses[4] == ON ? "ON" : "OFF");

  Serial.println("Sending data...");
  Serial.println(postData);

  // Send POST request
  http.begin(serverAddress, serverPort, endpoint);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int httpResponseCode = http.POST(postData);

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    String response = http.getString();
    Serial.println(response);
  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }

  http.end();
}
