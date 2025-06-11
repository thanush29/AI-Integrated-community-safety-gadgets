// House 1 code

#define BLYNK_TEMPLATE_ID "User_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "Dhanush Raj House 1"
#define BLYNK_AUTH_TOKEN "User_AUTH_TOKEN"

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include <BlynkSimpleEsp8266.h>

#define SCREEN_WIDTH 128   
#define SCREEN_HEIGHT 64   
#define OLED_RESET -1      

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Sensor & Button Pins
#define FLAME_SENSOR_PIN D3  
#define GAS_SENSOR_PIN A0
#define ALERT_PIN D4  
#define BUZZER_PIN D9  
#define THEFT_BUTTON_PIN D5  
#define HEALTH_BUTTON_PIN D6  
#define ACKNOWLEDGE_BUTTON_PIN D7  

const int GAS_THRESHOLD = 800;

const char* ssid = "vivo V21e 5G";
const char* password = "sandy123";
const char* mqtt_server = "7730fe61f8d9449c92d9e149f031efcd.s1.eu.hivemq.cloud";
const char* mqtt_username = "nodemcu";
const char* mqtt_password = "Pass@123";
const int mqtt_port = 8883;

const char* house1 = "House1";
String house = String(house1);

bool acknowledged = false;
WiFiClientSecure espClient;
PubSubClient client(espClient);
BlynkTimer timer;

void setup_wifi() {
  Serial.print("Connecting to WiFi...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected! IP: " + WiFi.localIP().toString());
}

void callback(char* topic, byte* payload, unsigned int length) {
  String message = "";
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.println("Message received: " + message);

  displayAlert(message.c_str());

  if(message.substring(8, 14)!="Sorry"){
    activateAlert();
  } else {
    deactivateAlert();
  }
  
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP8266Client-" + String(random(0xffff), HEX);
    if (client.connect(clientId.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("Connected to MQTT");
      client.subscribe("alert");
    } else {
      Serial.print("Failed (code ");
      Serial.print(client.state());
      Serial.println(") retrying in 5s...");
      delay(5000);
    }
  }
}

void sendAlert(const char* house, const char* hazard) {
  char msg[100];
  snprintf(msg, 100, "%s: %s detected!", house, hazard);
  client.publish("alert", msg);
  Serial.println(msg);

  // Send notification to Blynk App
  Blynk.logEvent("security_alert", msg);

  // Display latest notification
  Blynk.virtualWrite(V6, msg);

  // Log the message in a history table
  Blynk.virtualWrite(V7, msg);  // Add alert to list

  // Clear the main notification after 5 seconds using a timer
  timer.setTimeout(5000L, []() {
    Blynk.virtualWrite(V6, " ");  // Clear notification
  });
}



// Blynk Button States
bool theftAlert = false;
bool medicalAlert = false;
bool acknowledgment = false;

// Get Button States from Blynk Web App
BLYNK_WRITE(V1) {
  theftAlert = param.asInt();  // Read Theft Button from Web App
}

BLYNK_WRITE(V2) {
  medicalAlert = param.asInt();  // Read Medical Button from Web App
}

BLYNK_WRITE(V3) {
  acknowledgment = param.asInt();  // Read Acknowledge Button from Web App
}


void activateAlert() {
  digitalWrite(ALERT_PIN, HIGH);

  // Increased frequency beeping (2000Hz, 5 times)
  for (int i = 0; i < 5; i++) {
    tone(BUZZER_PIN, 2000, 700);
    delay(1000);
  }

  acknowledged = false;
}

void deactivateAlert() {
  digitalWrite(ALERT_PIN, LOW);
  noTone(BUZZER_PIN);
}

void displayAlert(const char* message) {
  String msg = String(message);  // Convert to String object
  if (msg.substring(0, 6) == house){
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println("        "+house);
    display.println("Status:");
    display.println(msg.substring(8));
    display.println("Alert Sending...");
  } else {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("        "+house);
  display.println("ALERT: ");
  display.println(msg);  // Display full message
  display.display(); }
}

void updateBlynk() {
  int gasLevel = analogRead(GAS_SENSOR_PIN);
  Blynk.virtualWrite(V4, gasLevel);

  int flameState = digitalRead(FLAME_SENSOR_PIN);
  if (flameState == LOW) {
    Blynk.virtualWrite(V5, "🔥 Fire Detected!");
  } else {
    Blynk.virtualWrite(V5, "Safe ✅");
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(FLAME_SENSOR_PIN, INPUT);
  pinMode(GAS_SENSOR_PIN, INPUT);
  pinMode(ALERT_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(THEFT_BUTTON_PIN, INPUT_PULLUP);
  pinMode(HEALTH_BUTTON_PIN, INPUT_PULLUP);
  pinMode(ACKNOWLEDGE_BUTTON_PIN, INPUT_PULLUP);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("❌ OLED Init Failed!");
    while (true);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("System Initializing...");
  display.display();
  delay(2000);

  setup_wifi();
  espClient.setInsecure();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);
  timer.setInterval(2000L, updateBlynk);  // Update Blynk every 2 seconds
}

void loop() {
  if (!client.connected()) reconnect();
  client.loop();
  Blynk.run();
  timer.run();

  

  int flameState = digitalRead(FLAME_SENSOR_PIN);
  int gasLevel = analogRead(GAS_SENSOR_PIN);

  bool flameDetected = (flameState == LOW);
  bool gasLeakDetected = (gasLevel > GAS_THRESHOLD);

  Serial.print("Gas Level: ");
  Serial.println(gasLevel);

  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("        "+house);
  display.println("Status:");

  if (flameDetected || gasLeakDetected) {
    // display.println("⚠ ALERT DETECTED!");
    activateAlert();
    if (flameDetected) {
      Serial.println("🔥 Fire Detected!");
      display.println("Fire Detected!");
      display.println("Alert Sending...");
      sendAlert(house1, "Fire");
    }
    if (gasLeakDetected) {
      Serial.println("💨 Gas Leak Detected!");
      display.println("Gas Leak Detected!");
      display.println("Alert Sending...");
      display.print("Gas Level: ");
      display.println(gasLevel);
      sendAlert(house1, "Gas Leak");
    }
  } else if (digitalRead(THEFT_BUTTON_PIN) == LOW || theftAlert) {  // BUTTONS start from here
    Serial.println("🚨 Theft Alert Triggered!");
    display.println("Theft Detected!");
    display.println("Alert Sending...");
    sendAlert(house1, "Theft Emergency");
    Blynk.virtualWrite(V1, 1);  // Sync Web App Button
    delay(500);
  } else if (digitalRead(HEALTH_BUTTON_PIN) == LOW || medicalAlert) {
    Serial.println("🏥 Medical Alert Triggered!");
    display.println("Medical Emergency Detected!");
    display.println("Alert Sending...");
    sendAlert(house1, "Medical Emergency");
    Blynk.virtualWrite(V2, 1);  // Sync Web App Button
    delay(500);
  } else {
    Serial.println("✅ Safe");
    display.println("Safe Environment");
    display.print("Gas Level: ");
    display.println(gasLevel);
    deactivateAlert();
  }

  if (digitalRead(ACKNOWLEDGE_BUTTON_PIN) == LOW || acknowledgment) {
    acknowledged = true;
    deactivateAlert();
    Serial.println("✅ Alert Acknowledged!"); 
    display.println("Acknowledgement Sending...");
    sendAlert(house1, "Sorry the False Alert..");
    Blynk.virtualWrite(V3, 1);  // Sync Web App Button
    delay(500);
  }

  display.display();
  delay(500);
}