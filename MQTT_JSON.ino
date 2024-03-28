#include <ArduinoJson.h>
#include <WiFi.h>
#include <PubSubClient.h>

// WiFi
const char *ssid = "Saturdays"; // Enter your Wi-Fi name
const char *password = "everydaysaturdays";  // Enter Wi-Fi password

// MQTT Broker
const char *mqttServer = "192.168.1.27";
const char *mqttUser = "admin1";
const char *mqttPassword = "admin1";
const int mqttPort = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println();
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.println("Connected to the WiFi network");
 
  client.setServer(mqttServer, mqttPort);
 
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
 
    if (client.connect("ESP32Client", mqttUser, mqttPassword )) {
 
      Serial.println("connected");
 
    } else {
 
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
 
    }
  }

}

void loop() {
  // put your main code here, to run repeatedly:
  StaticJsonBuffer<300> JSONbuffer;
  JsonObject& JSONencoder = JSONbuffer.createObject();
 
  JSONencoder["device"] = "ESP32";
  JSONencoder["sensorType"] = "Temperature";
  JsonArray& values = JSONencoder.createNestedArray("values");
 
  values.add(20);
  values.add(21);
  values.add(23);
 
  char JSONmessageBuffer[100];
  JSONencoder.printTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
  Serial.println("Sending message to MQTT topic..");
  Serial.println(JSONmessageBuffer);
 
  if (client.publish("esp/test", JSONmessageBuffer) == true) {
    Serial.println("Success sending message");
  } else {
    Serial.println("Error sending message");
  }
 
  client.loop();
  Serial.println("-------------");
 
  delay(10000);

}
