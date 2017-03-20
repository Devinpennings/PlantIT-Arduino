int incomingByte;
bool start = false;
bool einde = false;

void DebugModeChecker()
{
  if (Serial.available() > 0)
  {
    incomingByte = Serial.read();

    if (incomingByte == '#')
    {
      start = true;
      serialText = "";
      textReceived = false;
    }
    else if (incomingByte == '%')
    {
      einde = true;
    }
    else if (start && !einde)
    {
      char receivedCharacter = (char) incomingByte;
      serialText += receivedCharacter;
    }
    if (start && einde)
    {
      Serial.println(serialText);
      textReceived = true;
      start = false;
      einde = false;
      return serialText;
    }
  }
}

void DebugMode() {
  if (serialText == "WATER_VALUE") {
    Serial.print("\n#WATER:");
    Serial.print(analogRead(watersensor));
    Serial.println("%");
    serialText = "";
  }
  else if (serialText == "WATER_PLANT") {
    GeefWater();
    serialText = "";
  }
  else if (serialText == "SOIL_VALUE") {
    Serial.print("\n#SOIL:");
    Serial.print(analogRead(grondwater));
    Serial.println("%");
    serialText = "";
  }
  else if (serialText == "WIFI_STATUS") {
    printWifiStatus();
    serialText = "";
  }
  else if (serialText == "EXIT_DEBUG") {
    debugMode = false;
    serialText = "";
  }
}

