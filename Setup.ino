char ssid[] = "placeholder";
char pass[] = "placeholder";
int status = WL_IDLE_STATUS;

void wifiSetup() {
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("#WIFI SHIELD NOT PRESENT%");
    while (true);
  }

  while ( status != WL_CONNECTED) {
    Serial.print("#ATTEMPTING TO CONNECT TO:");
    Serial.print(ssid);
    Serial.println("...%");
    status = WiFi.begin(ssid, pass);
    delay(5000);
  }
  printWifiStatus();
}

void printWifiStatus() {
  Serial.print("#WIFI:");
  Serial.print(WiFi.status());
  Serial.print(":");
  Serial.print(ssid);
  Serial.print(":");
  Serial.print(WiFi.localIP());
  Serial.print(":");
  long rssi = WiFi.RSSI();
  Serial.print(rssi);
  Serial.println("dBm%");
}

void SetTime() {
  httpRequest(Tijd);
  complete = false;
  long startMillis = millis();
  long startTimeJson = millis();
  while (!complete) {
    jsonReader(Tijd);
    if (millis() > startMillis + 60000) {
      break;
    }
  }
  // seconds, minutes, hours, day of the week, day of the month, month, year
  myRTC.setDS1302Time(00, minuten, uren, 2, 10, 1, 2016);
  myRTC.updateTime();
  PrintTime();
  if (myRTC.minutes == 0 && myRTC.hours == 0) {
    SetTime();
  }
}

