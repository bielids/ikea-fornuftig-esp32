#include <WiFi.h>

WiFiServer server(80);

const int fanLow = 4;
const int fanMed = 0;
const int fanHigh = 2;

const int inputLow = 23;
const int inputMed = 19;
const int inputHigh = 18;

const int inWeb = 22;

// State of the push button
// int r1s = 0;
// int r2s = 0;
// int r3s = 0;
int inMedState = 0;
int inHighState = 0;
int inLowState = 0;
int inWebState = 0;
int overrideVal = 0;


const char* ssid = "Viscara";
const char* password = "Hectic cosmic soup! Again?";

void setup(){
    Serial.begin(115200);
    delay(1000);
    
    WiFi.mode(WIFI_STA); //Optional
    WiFi.begin(ssid, password);
    Serial.println("\nConnecting");

    while(WiFi.status() != WL_CONNECTED){
        Serial.print(".");
        delay(100);
    }

    Serial.println("\nConnected to the WiFi network");
    Serial.print("Local ESP32 IP: ");
    Serial.println(WiFi.localIP());

    server.begin();

    //Set the pin as an input pullup
    pinMode(fanLow, OUTPUT);
    pinMode(fanMed, OUTPUT);
    pinMode(fanHigh, OUTPUT);
    pinMode(inputMed, INPUT_PULLUP);
    pinMode(inputHigh, INPUT_PULLUP);
    pinMode(inputLow, INPUT_PULLUP);
    pinMode(inWeb, INPUT_PULLUP);
}

void loop() {

    inMedState = digitalRead(inputMed);
    inHighState = digitalRead(inputHigh);
    inLowState = digitalRead(inputLow);
    inWebState = digitalRead(inWeb);

    while (!inWebState) {
//      Serial.println("In while loop");
      if (overrideVal == 0)
      {
        digitalWrite(fanLow, HIGH);
        digitalWrite(fanMed, HIGH);
        digitalWrite(fanHigh, HIGH);
      }

      overrideVal = 1;
      delay(300);
      WiFiClient client = server.available();
// do nothing if no client is found 
      if (!client) {
          return;
      }

      Serial.println('Found client');
  
      while(!client.available()){
      }
  
      String request = client.readStringUntil('\r');
      Serial.println(request);
      client.flush();
  
      int value = 0;
  
      if (request.indexOf("/FAN=OFF") != -1) 
      {
        digitalWrite(fanLow, HIGH);
        digitalWrite(fanMed, HIGH);
        digitalWrite(fanHigh, HIGH);
        value = 2;
      } 
  
      if (request.indexOf("/FAN=HIGH") != -1) 
      {
        digitalWrite(fanLow, LOW);
        digitalWrite(fanMed, HIGH);
        digitalWrite(fanHigh, HIGH);
        value = 2;
      } 
  
      if (request.indexOf("/FAN=MED") != -1) 
      {
        digitalWrite(fanLow, HIGH);
        digitalWrite(fanMed, LOW);
        digitalWrite(fanHigh, HIGH);
        value = 2;
      } 
      
      if (request.indexOf("/FAN=LOW") != -1) 
      {
        digitalWrite(fanLow, HIGH);
        digitalWrite(fanMed, HIGH);
        digitalWrite(fanHigh, LOW);
        value = 2;
      } 
    
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println(""); //  do not forget this one
    
      client.println("<!DOCTYPE HTML>");
      client.println("<html>");
      client.print("FAN status: "); 
    
      if(value == LOW) 
      {
        client.print("ON");  
      } 
      else 
      {
        client.print("OFF");
      }
      client.println("<br><br>");
      client.println("Turn <a href=\"/FAN=ON\">ON</a><br>");
      client.println("Turn <a href=\"/FAN=OFF\">OFF</a><br>");
      client.println("</html>");
    
//      Serial.println("");
      delay(100);
    }

//    Serial.println("Not off, checking input vals");
    overrideVal = 0;

    inMedState = digitalRead(inputMed);
    inHighState = digitalRead(inputHigh);
    inLowState = digitalRead(inputLow);
    inWebState = digitalRead(inWeb);

//    Serial.println(inLowState);
//    Serial.println(inMedState);
//    Serial.println(inHighState);
    delay(100);

    if (inMedState < 1) {
//        Serial.println("MED SPEED");
        digitalWrite(fanMed, LOW);
    } else {
        digitalWrite(fanMed, HIGH);
    }
    
    // 
    if (inLowState < 1) {
//        Serial.println("HIGH SPEED");
        digitalWrite(fanLow, LOW);
    } else {
        digitalWrite(fanLow, HIGH);
    }

   
    if (inHighState < 1) {
//          Serial.println("LOW SPEED");
          digitalWrite(fanHigh, LOW);
    } else {
          digitalWrite(fanHigh, HIGH);
    }
    
    inWebState = digitalRead(inWeb);
    delay(100);
}
