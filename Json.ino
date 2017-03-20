int endResponse = 0;
boolean startJson = false;

void parseJson(const char * jsonString, requests doc) {
  StaticJsonBuffer<200> jsonBuffer;

  JsonObject& root = jsonBuffer.parseObject(jsonString);

  if (!root.success()) {
    return;
  }

  if (doc == Tijd) {
    weekuren = root["weekuren"];
    uren = root["uur"];
    minuten = root ["minuten"];
  }
  else if (doc == Watertijd) {
    watertijd[0] = root["Uren"][0];
    watertijd[1] = root["Uren"][1];
  }
  else if (doc == GeefNuWater) {
    nuWater = root["result"];
  }
}

void jsonReader(requests doc) {
  char c = 0;
  if (client.available()) {
    c = client.read();
    Serial.print(c);
    if (c == '{') {
      startJson = true;
      endResponse++;
      complete = false;
    }
    if (c == '}') {
      endResponse--;
    }
    if (startJson == true) {
      text += c;
    }
    if (endResponse == 0 && startJson == true) {
      parseJson(text.c_str(), doc);
      text = "";
      startJson = false;
      complete = true;
    }
  }
}

