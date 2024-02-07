#include <Arduino.h>
#include <RTClib.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPI.h>
#include <NTPClient.h> 
#include <ESP8266WiFi.h> 
#include <WiFiUdp.h>
#include <TimeLib.h>

#define LOCAL_SSID "QUE-STARLINK"
#define LOCAL_PASS "Quefamily01259"

RTC_DS1307 rtc; 
DateTime dtnow; 

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
void printTime(int year, int month, int day, int hour, int minute, int second);
void printRTCTime(DateTime datetime); 
void printNTPTime(NTPClient timeClient);
void adjustRTCWithNTP(NTPClient timeClient, RTC_DS1307 rtc);

void setup() {
  Serial.begin(115200); 

  // init RTC 
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC.");
    while (1);
  }

/*
on startup, pull time from RTC.
when internet connected, update RTC with NTP. 
when internet not connected, get time from RTC. 
*/
  Serial.print("Connecting to "); 
  Serial.println(LOCAL_SSID);

  // if (!WiFi.config(local_IP, gateway, subnet)) {
  //   Serial.println("Station failed to configure.");
  // }
  
  WiFi.begin(LOCAL_SSID, LOCAL_PASS); 
  // while (WiFi.status() != WL_CONNECTED) {
  //   delay(500); 
  //   Serial.print(".");
  // }
  //  print local IP address and start web server 
  printWiFi();

  // check if the ESP has internet access 


  timeClient.begin();
  timeClient.setTimeOffset(UTCOffsetInSeconds); // GMT+8
}

void loop() {
  // put your main code here, to run repeatedly:
  timeClient.update();
  bool NTPUpdateStatus = timeClient.isTimeSet();
  printNTPTime(timeClient);
  Serial.print("NTP update status: ");
  Serial.println(NTPUpdateStatus);
  if (NTPUpdateStatus) {adjustRTCWithNTP(timeClient, rtc);}
  dtnow = rtc.now(); 
  printRTCTime(dtnow);
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
  printTime(datetime.year(), datetime.month(), datetime.day(), 
    datetime.hour(), datetime.minute(), datetime.second());
  // Serial.print(datetime.dayOfTheWeek(), DEC); 
  // Serial.print(' '); 
  // Serial.println(daysOfTheWeek[datetime.dayOfTheWeek()]);
  // Serial.println();
}

void printNTPTime(NTPClient timeClient) {
  unsigned long epochTime = timeClient.getEpochTime();
  Serial.println("NTP time: ");
  printTime(year(epochTime), month(epochTime), day(epochTime), 
    timeClient.getHours(), timeClient.getMinutes(), timeClient.getSeconds());
  // Serial.print(timeClient.getFormattedTime());
  // Serial.print(' ');
  // Serial.print(timeClient.getDay());
  // Serial.println();
}

void printTime(int year, int month, int day, int hour, int minute, int second) {
  Serial.print(year);
  Serial.print('/');
  Serial.print(month);
  Serial.print('/');
  Serial.println(day);
  Serial.print(' ');
  Serial.print(hour); 
  Serial.print(":");
  Serial.print(minute); 
  Serial.print(":");
  Serial.print(second); 
  Serial.println();
}

void adjustRTCWithNTP(NTPClient timeClient, RTC_DS1307 rtc) {
  unsigned long epochTime = timeClient.getEpochTime();
  int _year = year(epochTime);
  int _month = month(epochTime);
  int _day = day(epochTime);
  int hour = timeClient.getHours();
  int minute = timeClient.getMinutes();
  int second = timeClient.getSeconds();
  rtc.adjust(DateTime(_year, _month, _day, hour, minute, second));
  Serial.println("time adjusted from NTP to RTC.");
}