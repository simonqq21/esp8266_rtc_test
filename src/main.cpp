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

const long UTCOffsetInSeconds = 28800;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "ntp.pagasa.dost.gov.ph");

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
void printRTCTime(DateTime datetime); 
void printNTPTime(NTPClient timeClient);

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

  timeClient.begin();
  timeClient.setTimeOffset(UTCOffsetInSeconds);
}

void loop() {
  // put your main code here, to run repeatedly:
  timeClient.update();
  printNTPTime(timeClient);
  now = rtc.now(); 
  printRTCTime(now);
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

void printRTCTime(DateTime datetime) {
  Serial.println("RTC time: ");
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
  Serial.println();
}

void printNTPTime(NTPClient timeClient) {
  Serial.println("NTP time: ");
  Serial.println(timeClient.getEpochTime()); 
  Serial.print(timeClient.getHours()); 
  Serial.print(":");
  Serial.print(timeClient.getMinutes()); 
  Serial.print(":");
  Serial.print(timeClient.getSeconds()); 
  Serial.print(' ');
  Serial.print(timeClient.getFormattedTime());
  Serial.print(' ');
  Serial.print(timeClient.getDay());
  Serial.println();
}