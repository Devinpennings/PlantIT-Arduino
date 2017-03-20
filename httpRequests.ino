const char server[] = "placeholder";

void httpRequest(requests doc) {
  client.stop();

  if (client.connect(server, 80)) {
    client.print("GET ");
    if (doc == Tijd) {
      client.print("/time");
    }
    else if (doc == Watertijd) {
      client.print("/arduino/plant/1/getWatertijd");
    }
    else if (doc == GeefNuWater) {
      client.print("/plant/1/getWaterNow");
    }
    else if (doc == DisableWaterNow) {
      client.print("/plant/1/disableWaterNow");
    }
    else if (doc == WaterGegeven) {
      client.print("/plant/1/WaterGegeven");
    }
    client.println(" HTTP/1.1");
    client.println("Host: placeholder");
    client.println("User-Agent: ArduinoWiFi/1.1");
    client.println("Connection: close");
    client.println();
  }
  else {
    Serial.println("#CONNECTION FAILED%");
  }
}

void httpRequest(requests doc, int waarde) {
  client.stop();

  if (client.connect("placeholder", 80)) {
    client.print("GET ");
    if (doc == Waterlevel) {
      client.print("/plant/1/waterlevel/");
      client.print(waarde);
    }
    else if (doc == Vochtigheid) {
      client.print("/plant/1/vochtigheid/");
      client.print(waarde);
    }
    client.println(" HTTP/1.1");
    client.println("Host: placeholder");
    client.println("User-Agent: ArduinoWiFi/1.1");
    client.println("Connection: close");
    client.println();
  }
  else {
    Serial.println("#CONNECTION FAILED%");
  }
}

