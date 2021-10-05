#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = "f9363a2ec8a345d4901324b529b16f4d";
char ssid[] = "OPENFAB_Egitim";
char pass[] = "Openfab2018";

#define BLYNK_PRINT Serial 

String one;
String two;
String three;

void setup(){
  Serial.begin(9600);
  Blynk.begin(auth,ssid,pass);
  
}

void loop(){

  Blynk.run();
}

BLYNK_WRITE(V0){
  one = param[0].asString();
  two = param[1].asString();
  three = param[2].asString();
  Serial.println(one);
  Serial.println(two);
  Serial.println(three);
}

