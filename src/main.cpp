#include <Arduino.h>
#include <RTClib.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPI.h>
#include <NTPClient.h> 
#include <ESP8266WiFi.h> 
#include <WiFiUdp.h>

#define LOCAL_SSID "QUE-STARLINK"
#define LOCAL_PASS "Quefamily01259"

RTC_DS1307 rtc; 
DateTime now; 

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

String daysOfTheWeek[7] = {
  "Monday", 
  "Tuesday", 
  "Wednesday",
  "Thursday",
  "Friday",
  "Saturday", 
  "Sunday"
};

// put function declarations here:
void printWiFi();
void printDateTime(DateTime datetime); 

void setup() {
  Serial.begin(115200); 

  // init RTC 
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC.");
    while (1);
  }

  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));  

  Serial.print("Connecting to "); 
  Serial.println(LOCAL_SSID);

  // if (!WiFi.config(local_IP, gateway, subnet)) {
  //   Serial.println("Station failed to configure.");
  // }
  
  WiFi.begin(LOCAL_SSID, LOCAL_PASS); 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); 
    Serial.print(".");
  }
  //  print local IP address and start web server 
  printWiFi();
}

void loop() {
  // put your main code here, to run repeatedly:
  now = rtc.now(); 
  printDateTime(now);
  delay(1000);
}

// put function definitions here:
void printWiFi() {
  Serial.println(" ");
  Serial.println("WiFi connected.");
  Serial.print("WiFi SSID: ");
  Serial.println(WiFi.SSID());
  Serial.print("IP address: "); 
  Serial.println(WiFi.localIP()); 
  long rssi = WiFi.RSSI(); 
  Serial.print("Signal strength (RSSI): "); 
  Serial.print(rssi);
  Serial.println(" dBm");
}

void printDateTime(DateTime datetime) {
  Serial.print(datetime.year(), DEC);
  Serial.print('/');
  Serial.print(datetime.month(), DEC);
  Serial.print('/');
  Serial.print(datetime.day(), DEC);
  Serial.print(' ');
  Serial.print(datetime.hour(), DEC); 
  Serial.print(':'); 
  Serial.print(datetime.minute(), DEC); 
  Serial.print(':'); 
  Serial.print(datetime.second(), DEC); 
  Serial.print(' '); 
  Serial.print(datetime.dayOfTheWeek(), DEC); 
  Serial.print(' '); 
  Serial.println(daysOfTheWeek[datetime.dayOfTheWeek()]);
}