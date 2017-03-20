void GeefWater() {
  Serial.println("\n\n#STARTED WATERING PLANT...%");
  long startmillis = millis();
  while (millis() < startmillis + 40000) {
    digitalWrite(pomp, HIGH);
  }
  digitalWrite(pomp, LOW);
  httpRequest(DisableWaterNow);
  Serial.println("#STOPPED WATERING PLANT...%");
  PushWaterlevel();
  PushGrondwater();
}

bool IsHetAlWaterTijd() {
  bool bl = false;
  httpRequest(Watertijd);
  long timeout = millis();
  while (!client.available() && millis() < timeout + 15000) {}
  complete = false;
  long startMillis = millis();
  long startTimeJson = millis();
  while (!complete) {
    jsonReader(Watertijd);
    if (millis() > startMillis + 10000) {
      break;
    }
  }
  for (int i = 0; i < 7; i++) {
    if (watertijd[i] == weekuren) {
      bl = true;
    }
  }
  return bl;
}

