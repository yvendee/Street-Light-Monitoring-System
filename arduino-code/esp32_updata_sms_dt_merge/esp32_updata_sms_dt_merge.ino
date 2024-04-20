#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "your_wifi";
const char* password = "your_password";
const char* serverAddress = "streetlightmonitoring2024.000webhostapp.com";
const int serverPort = 80;
const char* endpointDatetime = "/datetime.php";
const char* endpoint = "/post-sensordata-all.php";
const char* endpointSendSMS = "/sendsms.php";
const char* phoneNumber = "639095080459";
char* imessage;

int nightmode = 0;

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

  connectToWiFi();
}

void loop() {
  // Read sensor values
  int sensorValue1 = analogRead(LDRPIN_1);
  int sensorValue2 = analogRead(LDRPIN_2);
  int sensorValue3 = analogRead(LDRPIN_3);
  int sensorValue4 = analogRead(LDRPIN_4);
  int sensorValue5 = analogRead(LDRPIN_5);

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

  // Set LED status based on sensor values
  digitalWrite(LEDPIN_1, (sensorValue1 > 500) ? HIGH : LOW);
  digitalWrite(LEDPIN_2, (sensorValue2 > 500) ? HIGH : LOW);
  digitalWrite(LEDPIN_3, (sensorValue3 > 500) ? HIGH : LOW);
  digitalWrite(LEDPIN_4, (sensorValue4 > 500) ? HIGH : LOW);
  digitalWrite(LEDPIN_5, (sensorValue5 > 500) ? HIGH : LOW);

  // Send sensor data to the server
  sendSensorData(ledStatus, sensorValue1, sensorValue2, sensorValue3, sensorValue4, sensorValue5);
  delay(10000); 

  // Get and print datetime
  printDatetime();
  delay(10000);

  if (nightmode == 1) {
    Serial.println("Sending busted alert!");

    // Reset imessage
    if (imessage != nullptr) {
      delete[] imessage;
    }
    imessage = nullptr;

    // Allocate memory for imessage
    size_t bufferSize = 500; // Adjust buffer size as needed
    imessage = new char[bufferSize];
    strcpy(imessage, ""); // Initialize imessage as an empty string

    if (sensorValue1 > 500) {
      strcat(imessage, "post1(Bagani Campo) "); // Use strcat to concatenate strings
    }

    if (sensorValue2 > 500) {
      strcat(imessage, "post2(Bagani Gabor) ");
    }

    if (sensorValue3 > 500) {
      strcat(imessage, "post3(Oaig-Daya) ");
    }

    if (sensorValue4 > 500) {
      strcat(imessage, "post4(Bagani Ubbog) ");
    }

    if (sensorValue5 > 500) {
      strcat(imessage, "post5(Ayudante) ");
    }

    strcat(imessage, "busted bulb detected"); // Use strcat to concatenate strings

    // Send SMS
    sendSMS(imessage, phoneNumber);
    
    // Free memory for imessage
    delete[] imessage;
    imessage = nullptr;
    nightmode = 2;
  }

  delay(10000); // Adjust delay as needed
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

void printDatetime() {
  if (!client.connect(serverAddress, serverPort)) {
    Serial.println("Connection failed.");
    return;
  }

  // Make an HTTP GET request to fetch datetime
  HTTPClient http;
  http.begin(client, serverAddress, serverPort, endpointDatetime);
  int httpCode = http.GET();

  // Check for the response code
  if (httpCode > 0) {
    // Check for a valid response
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println("Datetime: " + payload);

      // Extract hour from datetime
      int hourIndex = payload.indexOf(":");
      if (hourIndex != -1) {
        int hour = payload.substring(hourIndex - 2, hourIndex).toInt();
        if (hour >= 18 || hour < 6) {
          Serial.println("It's night");
          if(nightmode == 0) nightmode = 1;
        }
        else {
          nightmode = 0;
        }
      }
    } else {
      Serial.print("HTTP request failed with error code: ");
      Serial.println(httpCode);
    }
  } else {
    Serial.println("HTTP request failed.");
  }

  http.end();
}

void sendSMS(const char* message, const char* phoneNumber) {
  if (!client.connect(serverAddress, serverPort)) {
    Serial.println("Connection failed.");
    return;
  }

  // Construct the POST data
  String postData = "message=" + String(message) + "&to-number=" + String(phoneNumber);

  // Make an HTTP POST request to send SMS
  HTTPClient http;
  http.begin(client, serverAddress, serverPort, endpointSendSMS);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int httpCode = http.POST(postData);

  // Check for the response code
  if (httpCode > 0) {
    // Check for a valid response
    if (httpCode == HTTP_CODE_OK) {
      Serial.println("SMS sent successfully");
    } else {
      Serial.print("HTTP request failed with error code: ");
      Serial.println(httpCode);
    }
  } else {
    Serial.println("HTTP request failed.");
  }

  http.end();
}
