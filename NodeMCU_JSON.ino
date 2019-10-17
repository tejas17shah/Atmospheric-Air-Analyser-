#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <SoftwareSerial.h>
#include "ThingSpeak.h"

SoftwareSerial mySerial(5, 4);
WiFiClient client; // Creating WiFiClient Object

//ThingSpeak Channel's API Keys
unsigned long myChannelNumber = enterChannelNumber;
const char * myWriteAPIKey = "enterYourWriteKey";

//Add your WiFi credentials here
#define WIFI_SSID "yourSSID"
#define WIFI_PASSWORD "yourPWD"


void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  ThingSpeak.begin(client);
}

void loop() {
  // Check WiFi Status
  while (mySerial.available())
  {
    const size_t capacity = JSON_OBJECT_SIZE(7) + 100;
    DynamicJsonBuffer jsonBuffer(capacity);

    //const char* json = Serial.rea
    JsonObject& root = jsonBuffer.parseObject(mySerial);
    if (!root.success()) {
      Serial.println("parseObject() failed");
      return;
    }

    float CO2 = root["CO2"];
    float CO = root["CO"];
    float CH4 = root["CH4"];
    float NOx = root["NOx"];
    float NH3 = root["NH3"];
    float H2S = root["H2S"];
    float VOC = root["VOC"];

    Serial.print(CO2, 5);  Serial.print(",");
    Serial.print(CO, 5);  Serial.print(",");
    Serial.print(CH4, 5);  Serial.print(",");
    Serial.print(NOx, 5);  Serial.print(",");
    Serial.print(NH3, 5);  Serial.print(",");
    Serial.print(H2S, 5); Serial.print(",");
    Serial.println(VOC, 5);

    ThingSpeak.setField(1, CO2);
    ThingSpeak.setField(2, CO);
    ThingSpeak.setField(3, CH4);
    ThingSpeak.setField(4, NOx);
    ThingSpeak.setField(5, NH3);
    ThingSpeak.setField(6, H2S);
    ThingSpeak.setField(7, VOC);
    ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  }
}
