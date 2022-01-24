#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>;
#include <WiFiClient.h>;
#include <DHT.h>;
#define DHTPIN 0
DHT dht(DHTPIN, DHT11);  
const unsigned char buzz =14;
const char* ssid = "Madaya"; 
const char* password = "1112223333";
int val;
int LDRpin = A0; //LDR Pin Connected at A0 Pin
String serverName = "http://iothealth.ddns.net:1142/values/";
WiFiClient client;
String apiKey = "197GD6ZDZT56AZM6"; 
unsigned long myChannelNumber = 1123344;
unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 5000;

void setup()
{
Serial.begin(9600);
delay(10);
dht.begin();
WiFi.begin(ssid, password);
pinMode(buzz,OUTPUT);
}

void loop()
{
val = analogRead(LDRpin); 
Serial.print(val); 
delay(1000);
float h = dht.readHumidity(); 
float t = dht.readTemperature();
     //Send an HTTP POST request every 10 minutes
  if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;

      String serverPath = serverName + "Temperature="+t+"&Humidity="+h;
      
      // Your Domain name with URL path or IP address with path
      http.begin(serverPath.c_str());
     
      // Send HTTP GET request
      int httpResponseCode = http.GET();
       if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();
    }else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}
