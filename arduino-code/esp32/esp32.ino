#include <WiFi.h>
#include <WiFiClient.h>

#define NUM_LEDS 5
#define LEDPIN_1 15
#define LEDPIN_2 16
#define LEDPIN_3 17
#define LEDPIN_4 18
#define LEDPIN_5 19
#define LDRPIN_1 34
#define LDRPIN_2 35
#define LDRPIN_3 36
#define LDRPIN_4 39
#define LDRPIN_5 32

const char* ssid = "your_wifi_ssid";
const char* password = "your_wifi_password";
const char* serverAddress = "streetlightmonitoring2024.000webhostapp.com";
const int serverPort = 80;
const char* endpoint = "/post-sensordata-all.php";

enum LEDStatus {
  OFF = 0,
  ON = 1
};

LEDStatus ledStatus[NUM_LEDS] = {OFF, OFF, OFF, OFF, OFF};

WiFiClient client;

void setup() {
  Serial.begin(115200);
  pinMode(LEDPIN_1, OUTPUT);
  pinMode(LEDPIN_2, OUTPUT);
  pinMode(LEDPIN_3, OUTPUT);
  pinMode(LEDPIN_4, OUTPUT);
  pinMode(LEDPIN_5, OUTPUT);
  pinMode(LDRPIN_1, INPUT);
  pinMode(LDRPIN_2, INPUT);
  pinMode(LDRPIN_3, INPUT);
  pinMode(LDRPIN_4, INPUT);
  pinMode(LDRPIN_5, INPUT);
  delay(1000);
  connectToWiFi();
}

void loop() {
  // Read sensor values
  int sensorValue1 = analogRead(LDRPIN_1);
  int sensorValue2 = analogRead(LDRPIN_2);
  int sensorValue3 = analogRead(LDRPIN_3);
  int sensorValue4 = analogRead(LDRPIN_4);
  int sensorValue5 = analogRead(LDRPIN_5);

  // Set LED status based on sensor values
  ledStatus[0] = (sensorValue1 > 500) ? ON : OFF;
  ledStatus[1] = (sensorValue2 > 500) ? ON : OFF;
  ledStatus[2] = (sensorValue3 > 500) ? ON : OFF;
  ledStatus[3] = (sensorValue4 > 500) ? ON : OFF;
  ledStatus[4] = (sensorValue5 > 500) ? ON : OFF;

  // Print sensor values
  Serial.print("LDR Value 1: ");
  Serial.println(sensorValue1);
  Serial.print("LDR Value 2: ");
  Serial.println(sensorValue2);
  Serial.print("LDR Value 3: ");
  Serial.println(sensorValue3);
  Serial.print("LDR Value 4: ");
  Serial.println(sensorValue4);
  Serial.print("LDR Value 5: ");
  Serial.println(sensorValue5);

  // Send sensor data to the server
  sendSensorData(ledStatus, sensorValue1, sensorValue2, sensorValue3, sensorValue4, sensorValue5);


  if(sensorValue1 > 500){
    digitalWrite(LEDPIN_1, HIGH);
    Serial.println("Low light detected. LED1s turned on.");
  }
  else{
    digitalWrite(LEDPIN_1, LOW);
    Serial.println("Sunlight detected. LED1s turned off.");
  }

  if(sensorValue2 > 500){
    digitalWrite(LEDPIN_2, HIGH);
    Serial.println("Low light detected. LED2s turned on.");
  }
  else{
    digitalWrite(LEDPIN_2, LOW);
    Serial.println("Sunlight detected. LED2s turned off.");
  }

  if(sensorValue3 > 500){
    digitalWrite(LEDPIN_3, HIGH);
    Serial.println("Low light detected. LED3s turned on.");
  }
  else{
    digitalWrite(LEDPIN_3, LOW);
    Serial.println("Sunlight detected. LED3s turned off.");
  }

  if(sensorValue4 > 500){
    digitalWrite(LEDPIN_4, HIGH);
    Serial.println("Low light detected. LED4s turned on.");
  }
  else{
    digitalWrite(LEDPIN_4, LOW);
    Serial.println("Sunlight detected. LED4s turned off.");
  }

  if(sensorValue5 > 500){
    digitalWrite(LEDPIN_5, HIGH);
    Serial.println("Low light detected. LED5s turned on.");
  }
  else{
    digitalWrite(LEDPIN_5, LOW);
    Serial.println("Sunlight detected. LED5s turned off.");
  }

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

void sendSensorData(LEDStatus statuses[], int sensorValue1, int sensorValue2, int sensorValue3, int sensorValue4, int sensorValue5) {
  if (!client.connect(serverAddress, serverPort)) {
    Serial.println("Connection failed.");
    delay(5000);
    return;
  }

  // Prepare data
  String postData = "statusforpostnum1=" + String(statuses[0] == ON ? "ON" : "OFF") +
                    "&statusforpostnum2=" + String(statuses[1] == ON ? "ON" : "OFF") +
                    "&statusforpostnum3=" + String(statuses[2] == ON ? "ON" : "OFF") +
                    "&statusforpostnum4=" + String(statuses[3] == ON ? "ON" : "OFF") +
                    "&statusforpostnum5=" + String(statuses[4] == ON ? "ON" : "OFF") +
                    "&Sensor1=" + String(sensorValue1) +
                    "&Sensor2=" + String(sensorValue2) +
                    "&Sensor3=" + String(sensorValue3) +
                    "&Sensor4=" + String(sensorValue4) +
                    "&Sensor5=" + String(sensorValue5);

  Serial.println("Sending data...");
  Serial.println(postData);

  // Send POST request
  client.print(String("POST ") + endpoint + " HTTP/1.1\r\n" +
               "Host: " + serverAddress + "\r\n" +
               "Content-Type: application/x-www-form-urlencoded\r\n" +
               "Content-Length: " + postData.length() + "\r\n" +
               "Connection: close\r\n" +
               "\r\n" + postData);

  delay(10);

  while (client.available()) {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  client.stop();
}
