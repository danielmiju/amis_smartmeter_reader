#include "proj.h"
// OpenDTU
//#define DEBUG
#ifndef DEBUG
  #define eprintf( fmt, args... )
  #define DBGOUT(...)
#else
  #if DEBUGHW>0
    #define FOO(...) __VA_ARGS__
    #define DBGOUT dbg_string+= FOO
    #if (DEBUGHW==2)
      #define eprintf(fmt, args...) S.printf(fmt, ##args)
    #elif (DEBUGHW==1 || DEBUGHW==3)
      #define eprintf(fmt, args...) {sprintf(dbg,fmt, ##args);dbg_string+=dbg;dbg[0]=0;}
    #endif
  #else
    #define eprintf( fmt, args... )
    #define DBGOUT(...)
  #endif
#endif

HTTPClient http;
WiFiClient client;
Ticker opendtutimer;

String opendtupower = "";
String opendtuyieldday = "";
int httpResponseCodeSO;

void startOpenDTUtimer(){
    opendtutimer.attach_scheduled(5, getOpenDTUdata);
}

void getOpenDTUdata(){
    if(config.opendtu_aktiv){
        //String ip = config.opendtu_ip;
        String pre = "http://";
        String post = "/api/livedata/status";
        String url = pre + config.opendtu_ip + post;
        const char *openDTUuri = url.c_str();

        String payload;
        JsonDocument doc;
        JsonObject json = doc.to<JsonObject>();

        http.begin(client, openDTUuri);
        httpResponseCodeSO = http.GET();

        if(httpResponseCodeSO == 200){
            payload = http.getString();
            deserializeJson(json, payload);
            float powerv = json["total"]["Power"]["v"];
            float yieldday = json["total"]["YieldDay"]["v"];
            opendtupower = String(powerv, 2);
            opendtuyieldday = String(yieldday, 0);
        }
        else{
            opendtupower = "Falsche IP oder OpenDTU nicht erreichbar!";
        }
        #ifdef DEBUG
            Serial.println(url);
            Serial.println(httpResponseCodeSO);
            Serial.println(opendtupower);
            Serial.println(opendtuyieldday);            
        #endif

        http.end();
    }
}
