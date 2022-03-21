#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include "web.h"

const char * ssid = "3Cs_LAB"; 
const char * pass = "3cslabsince2015"; 

WebServer server(80);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); 
  WiFi.mode(WIFI_STA); 
  WiFi.disconnect(); 
  WiFi.begin(ssid, pass); 
  while(WiFi.status() != WL_CONNECTED)
  {
    Serial.println("."); 
    delay(500);
  }
  Serial.println(" ");
  Serial.print(WiFi.localIP());

	server.on("/", [] {
		server.send(200, "text/html", webpage); 
  }); 

  server.on("/code", [] {
		Serial.println(server.arg("contents"));
		server.send(200, "text/html", webpage); 
  }); 
	server.begin(); 

}

void loop() {
  // put your main code here, to run repeatedly:
	server.handleClient();
}