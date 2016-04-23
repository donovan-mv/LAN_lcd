#include <Adafruit_CC3000.h>
#include <ccspi.h>
#include <SPI.h>
#include <LiquidCrystal.h>
#include "utility/debug.h"

LiquidCrystal lcd(8, 9, 6, 4, 7, 2);
Adafruit_CC3000 cc3000 = Adafruit_CC3000(10, 3, 5, SPI_CLOCK_DIVIDER); //replace with pin nos
#define ssid       "NETGEAR1"        
#define pass       "sunnypotato102"
#define sec        WLAN_SEC_WPA2

uint32_t ip = cc3000.IP2U32(10, 0, 0, 4);

void setup() {
  lcd.begin(16, 2);
  lcd.print("Initialising...");
  if (!cc3000.begin()){
    lcd.clear();
    lcd.print("Startup Error");
  }
  lcd.clear();
  lcd.print("CC3000 online!");
  
  cc3000.deleteProfiles(); //delete old profiles
  
  if (!cc3000.connectToAP(ssid, pass, sec, 3)) {
    lcd.clear(); lcd.print("Network conn err");
    while(1);
  }
   
  lcd.clear();
  lcd.print("Connected to AP");
}

String get_val(){
  lcd.clear();
  lcd.print("Sending req");
  Adafruit_CC3000_Client server = cc3000.connectTCP(ip, 8080);
  if(server.connected()){
    server.fastrprint(F("GET "));
    server.fastrprint(F("/getText"));
    server.fastrprint(F(" HTTP/1.2"));
    server.fastrprint(F("\n"));
    server.fastrprint(F("Connection: Close"));
    server.fastrprint(F("\n\n"));
  }
  
  String str = "";
  unsigned long e=millis();   //needed to check idle time and hence reduce latency
  while(server.connected()&&((millis()-e)<1500)){
    while(server.available()){
      char c=server.read();
      str+=c;
      e=millis();
    }
  }
  return str;
}

void disp(String data){
  int pos_2 = data.indexOf("\', \'");
  int pos_1 = 2;
  do{
    lcd.clear();
    lcd.print(data.substring(pos_1,pos_2));
    delay(3000);
    pos_1 = pos_2+4; //might be out of bounds
    pos_2 = data.indexOf("\', \'", pos_1-3);
  } while(pos_2 != -1);
}

void loop() {
  String val, text;
  val = get_val();
  text = val.substring(val.indexOf("[\'"),val.indexOf("\']")+3);
  //lcd.clear(); lcd.print(text); delay(3000);
  disp(text);
}
