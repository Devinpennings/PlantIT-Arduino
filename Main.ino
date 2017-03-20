// LIBRARIES
#include <virtuabotixRTC.h>
#include <SPI.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#define JSON_BUFF_DIMENSION 2500

// RealTimeClock
virtuabotixRTC myRTC(3, 5, 4);
int weekuren;
int uren;
int minuten;
int prevhours = 0;
int prevmins = 0;

// WATERING
bool nuWater = false;
int watertijd[7];

// WIFI
WiFiClient client;

// PINSETUP
const int pomp = 2;
const int grondwater = 0;
const int watersensor = 1;

// JSON
String text;
bool complete = false;
long startTimeJson;

// SERIAL
bool textReceived = true;
String serialText;
bool debugMode = false;

//HTTPREQUEST
enum requests {
  Tijd,
  Waterlevel,
  Vochtigheid,
  Watertijd,
  GeefNuWater,
  WaterGegeven,
  DisableWaterNow
};

void setup() {
  Serial.begin(9600);
  wifiSetup();
  pinMode(pomp, OUTPUT);

  text.reserve(JSON_BUFF_DIMENSION);
  
  SetTime();
}

void loop() {
  myRTC.updateTime();

  do {
    DebugModeChecker();
  }
  while (!textReceived);

  if (serialText == "DEBUG_MODE") {
    debugMode = true;
  }
  else if (debugMode) {
    DebugMode();
  }
  else {
    if (myRTC.seconds == 0 || myRTC.seconds == 15 || myRTC.seconds == 30 || myRTC.seconds == 45) {
      Serial.println("\n\n#15 SECONDS PASSED...%");
      httpRequest(GeefNuWater);
      complete = false;
      long startMillis = millis();
      long startTimeJson = millis();
      while (!complete) {
        jsonReader(GeefNuWater);
        if (millis() > startMillis + 10000) {
          break;
        }
      }
      if (nuWater) {
        GeefWater();
        httpRequest(DisableWaterNow);
        while (!client.available()) {}
      }
    }

    if (myRTC.hours <= 0)
    {
      prevhours = 0;
      SetTime();
    }

    if (myRTC.minutes <= 0) {
      prevmins = 0;
    }

    if (myRTC.minutes > prevmins)
    {
      Serial.println("\n#MINUTE PASSED...%");
      PrintTime();
      prevmins = myRTC.minutes;
      PushGrondwater();
      PushWaterlevel();
    }

    if (myRTC.hours > prevhours)
    {
      Serial.println("\n#HOUR PASSED...%");
      prevhours = myRTC.hours;
      if (IsHetAlWaterTijd()) {
        GeefWater();
      }
      weekuren++;
      for (int i = 0; i < 7; i++) {
        Serial.print(i);
        Serial.print(": ");
        Serial.println(watertijd[i]);
      }
    }
  }
}

void PrintTime() {
  myRTC.updateTime();
  Serial.print("\n#TIME: ");
  Serial.print(myRTC.hours);
  Serial.print(":");
  Serial.print(myRTC.minutes);
  Serial.println("%");
}

void PushGrondwater() {
  Serial.print("\n#SOIL:");
  Serial.print(analogRead(grondwater));
  Serial.println("%");
  httpRequest(Vochtigheid, analogRead(grondwater));
  long timeout = millis();
  while (!client.available() && millis() < timeout + 15000) {}
}

void PushWaterlevel() {
  Serial.print("\n#WATER:");
  Serial.print(analogRead(watersensor));
  Serial.println("%");
  httpRequest(Waterlevel, analogRead(watersensor));
  long timeout = millis();
  while (!client.available() && millis() < timeout + 15000) {}
}

