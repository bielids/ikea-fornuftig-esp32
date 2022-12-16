#include <WiFi.h>

WiFiServer server(80);

const int rightOne = 4;
const int rightTwo = 0;
const int rightThree = 2;

const int middleOne = 19;
const int middleTwo = 18;

const int leftOne = 23;
const int leftTwo = 22;

// State of the push button
int r1s = 0;
int r2s = 0;
int r3s = 0;
int m1s = 0;
int m2s = 0;
int l1s = 0;
int l2s = 0;


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
    pinMode(rightOne, OUTPUT);
    pinMode(rightTwo, OUTPUT);
    pinMode(rightThree, OUTPUT);
    pinMode(middleOne, INPUT_PULLUP);
    pinMode(middleTwo, INPUT_PULLUP);
    pinMode(leftOne, INPUT_PULLUP);
    pinMode(leftTwo, INPUT_PULLUP);
/*    pinMode(ledPin, OUTPUT) */
}

void loop() {

//    digitalWrite(rightOne, HIGH);
/*    r1s = digitalRead(rightOne);
    Serial.println("right 1 state: ");
    Serial.println(r1s);
*/
//     digitalWrite(rightTwo, HIGH);
/*    r2s = digitalRead(rightTwo);
    Serial.println("right 2 state: ");
    Serial.println(r2s);
*/
//    digitalWrite(rightThree, HIGH);
/*    r3s = digitalRead(rightThree);
    Serial.println("right 3 state: ");
    Serial.println(r3s);
*/    

/*
 * l1 loq
 * m1 mid 
 * m2 high
 * l2 off
 */

    m1s = digitalRead(middleOne);
    m2s = digitalRead(middleTwo);
    l1s = digitalRead(leftOne);
    l2s = digitalRead(leftTwo);

    while (!l2s) {
      Serial.println("Broke out of while loop");
      WiFiClient client = server.available();
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
        digitalWrite(rightOne, LOW);
        digitalWrite(rightTwo, LOW);
        digitalWrite(rightThree, LOW);
        value = 2;
      } 
  
      if (request.indexOf("/LED=LOW") != -1) 
      {
        digitalWrite(rightOne, HIGH);
        digitalWrite(rightTwo, LOW);
        digitalWrite(rightThree, LOW);
        value = 2;
      } 
  
      if (request.indexOf("/LED=MED") != -1) 
      {
        digitalWrite(rightOne, LOW);
        digitalWrite(rightTwo, HIGH);
        digitalWrite(rightThree, LOW);
        value = 2;
      } 
      
      if (request.indexOf("/LED=HIGH") != -1) 
      {
        digitalWrite(rightOne, LOW);
        digitalWrite(rightTwo, LOW);
        digitalWrite(rightThree, HIGH);
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
    }
    Serial.println("Not off, checking input vals");

    m1s = digitalRead(middleOne);
    m2s = digitalRead(middleTwo);
    l1s = digitalRead(leftOne);
    l2s = digitalRead(leftTwo);

    Serial.println(l1s);
    Serial.println(m1s);
    Serial.println(m2s);
    delay(100);

    if (m1s < 1) {
        Serial.println("MED SPEED");
        digitalWrite(rightTwo, HIGH);
    } else {
        digitalWrite(rightTwo, LOW);
    }
    
    // 
    if (l1s < 1) {
        Serial.println("HIGH SPEED");
        digitalWrite(rightThree, HIGH);
    } else {
        digitalWrite(rightThree, LOW);
    }

   
    if (m2s < 1) {
          Serial.println("LOW SPEED");
          digitalWrite(rightOne, HIGH);
    } else {
          digitalWrite(rightOne, LOW);
    }
    
    l2s = digitalRead(leftTwo);
    delay(100);


//    Serial.println(" left 1 state: ");
//    Serial.println(l1s);
    
//   

//    Serial.println(" left 1 state: ");
//    Serial.println(l2s);

//    Serial.print('============================');

    

    delay(100);
/*    if (buttonState == LOW) {
        // Switch on the led
        digitalWrite(ledPin, HIGH);
    } else {
        // Switch off the led
        digitalWrite(ledPin, LOW);
    }*/
}
