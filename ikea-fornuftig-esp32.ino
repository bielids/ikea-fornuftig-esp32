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
      Serial.println("Broke out of while loop");
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
  
      if (request.indexOf("/LED=OFF") != -1) 
      {
        digitalWrite(fanLow, LOW);
        digitalWrite(fanMed, LOW);
        digitalWrite(fanHigh, LOW);
        value = 2;
      } 
  
      if (request.indexOf("/LED=LOW") != -1) 
      {
        digitalWrite(fanLow, HIGH);
        digitalWrite(fanMed, LOW);
        digitalWrite(fanHigh, LOW);
        value = 2;
      } 
  
      if (request.indexOf("/LED=MED") != -1) 
      {
        digitalWrite(fanLow, LOW);
        digitalWrite(fanMed, HIGH);
        digitalWrite(fanHigh, LOW);
        value = 2;
      } 
      
      if (request.indexOf("/LED=HIGH") != -1) 
      {
        digitalWrite(fanLow, LOW);
        digitalWrite(fanMed, LOW);
        digitalWrite(fanHigh, HIGH);
        value = 2;
      } 
    
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println(""); //  do not forget this one
    
      client.println("<!DOCTYPE HTML>");
      client.println("<html>");
      client.print("LED status: "); 
    
      if(value == HIGH) 
      {
        client.print("ON");  
      } 
      else 
      {
        client.print("OFF");
      }
      client.println("<br><br>");
      client.println("Turn <a href=\"/LED=ON\">ON</a><br>");
      client.println("Turn <a href=\"/LED=OFF\">OFF</a><br>");
      client.println("</html>");
    
      Serial.println("");
      delay(100);
    }

    Serial.println("Not off, checking input vals");

    inMedState = digitalRead(inputMed);
    inHighState = digitalRead(inputHigh);
    inLowState = digitalRead(inputLow);
    inWebState = digitalRead(inWeb);

    Serial.println(inLowStat);
    Serial.println(inMedStat);
    Serial.println(inHighStat);
    delay(100);

    if (inMedState < 1) {
        Serial.println("MED SPEED");
        digitalWrite(fanMed, HIGH);
    } else {
        digitalWrite(fanMed, LOW);
    }
    
    // 
    if (inLowState < 1) {
        Serial.println("LOW SPEED");
        digitalWrite(fanLow, HIGH);
    } else {
        digitalWrite(fanLow, LOW);
    }

   
    if (inHighState < 1) {
          Serial.println("HIGH SPEED");
          digitalWrite(fanHigh, HIGH);
    } else {
          digitalWrite(fanHigh, LOW);
    }
    
    inWebState = digitalRead(inWeb);
    delay(100);
}
