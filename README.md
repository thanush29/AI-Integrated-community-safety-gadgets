📌 Project Overview
This project is part of a smart community safety system that integrates AI, IoT, and real-time communication to respond to home hazards like gas leaks, fire, theft, and medical emergencies. The system monitors environmental parameters and emergency buttons, and sends immediate alerts via MQTT, OLED display, Buzzer, and the Blynk IoT app. It is also tested in Proteus simulation and deployed with Arduino Uno + ESP8266 hardware.
🧠 Key Features
•	🔥 Flame and Gas Detection using sensors
•	🚨 Emergency Buttons for:
o	Theft Alert
o	Medical Emergency
o	Manual Acknowledgement
•	🖥️ OLED Display for real-time status
•	📢 Buzzer & LED Alerts for local notifications
•	🌐 MQTT-based Messaging between devices (via HiveMQ Cloud)
•	📱 Blynk IoT App Integration for:
o	Alerts
o	Live sensor readings
o	Remote triggering
•	🧪 Tested in Proteus and deployed on real hardware
🧰 Hardware Components
•	ESP8266 (NodeMCU)
•	Arduino Uno
•	Flame Sensor (Digital)
•	Gas Sensor (MQ-2 or similar)
•	Push Buttons (Theft, Medical, Acknowledge)
•	OLED Display (128x64, I2C SSD1306)
•	Buzzer
•	LED or Alert Indicator
•	Resistors, Breadboard, Jumper Wires
📡 Software & Libraries
•	Arduino IDE
•	Proteus 8 Professional (for simulation)
•	Blynk IoT Platform
•	Libraries Used:
o	Adafruit_SSD1306
o	Adafruit_GFX
o	BlynkSimpleEsp8266
o	ESP8266WiFi
o	PubSubClient
o	WiFiClientSecure
🔄 Functional Flow
1.	Startup: Connects to WiFi and MQTT broker.
2.	Monitoring: Continuously checks flame and gas sensors.
3.	Emergency Detection:
o	If gas level exceeds threshold or flame is detected, an alert is triggered.
o	Button press also triggers respective alerts.
4.	Alert System:
o	Sends MQTT message to community network
o	Displays message on OLED
o	Activates buzzer and LED
o	Pushes notification through Blynk App
5.	Acknowledgment:
o	Manual or remote acknowledgment deactivates alert and sends "false alert" message.
🔧 How to Run
1.	Upload the provided Arduino sketch to your ESP8266.
2.	Connect the sensors and modules as per your circuit.
3.	Install all required libraries in Arduino IDE.
4.	Configure your WiFi SSID/Password, MQTT Broker, and Blynk Auth Token in the code.
5.	Use the Blynk Web Dashboard or mobile app to monitor alerts and test remote control.
🔒 MQTT Configuration (HiveMQ Cloud)
•	Broker: 7730fe61f8d9449c92d9e149f031efcd.s1.eu.hivemq.cloud
•	Port: 8883
•	Username: nodemcu
•	Password: Pass@123
•	Topic: alert
📱 Blynk Virtual Pins
Virtual Pin	Function
V1	Theft Alert Button (Web/App)
V2	Medical Alert Button
V3	Acknowledge Alert Button
V4	Gas Sensor Reading
V5	Flame Sensor Status
V6	Main Alert Notification
V7	Alert History List
📊 Use Cases
•	Smart home fire and gas detection
•	Elderly or medical emergency alert system
•	Real-time theft detection in communities
•	Inter-house communication using MQTT
🎯 Project Goals
•	Improve real-time community safety
•	Integrate affordable IoT devices for home security
•	Align with SDG 11 (Sustainable Cities) and SDG 9 (Innovation)

