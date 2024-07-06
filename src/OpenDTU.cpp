#include "proj.h"
// OpenDTU
//#include <ESP8266HTTPClient.h>
//#include <ArduinoJson.h>

HTTPClient http;
WiFiClient client;

char openDTU[] = "http://10.70.70.25/api/livedata/status";

String DTUresult = "";

void livedata(){
    String payload;
    JsonDocument doc;
    JsonObject json = doc.to<JsonObject>();

    http.begin(client, openDTU);
    http.GET();
    //int httpResponseCodeSO = http.GET();
    payload = http.getString();
    //DeserializationError error = deserializeJson(docPM, payload);

    float result = json["total"]["Power"]["v"];

    DTUresult = String(result, 2);

    //Serial.print("PV Leistung: ");
    //Serial.println(DTUresult);
    //Serial.println(httpResponseCodeSO);

    http.end();
}