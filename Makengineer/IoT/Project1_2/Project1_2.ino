#include <Adafruit_NeoPixel.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <TimeLib.h>
#include <WidgetRTC.h>
#include <OneWire.h>
#include <DallasTemperature.h>

char auth[] = "f9363a2ec8a345d4901324b529b16f4d";
char ssid[] = "OPENFAB_Egitim";
char pass[] = "Openfab2018";

#define BUZZER D3
#define BUTTON D5
#define PIXELS_PIN 12
#define NUM_LEDS   24
#define BRIGHTNESS 30
#define PIXEL_TYPE NEO_GRB + NEO_KHZ800
#define BLYNK_PRINT Serial

Adafruit_NeoPixel ring = Adafruit_NeoPixel(NUM_LEDS, PIXELS_PIN, PIXEL_TYPE);

#define ONE_WIRE_BUS 2
OneWire oneWire(ONE_WIRE_BUS); 
DallasTemperature DS18B20(&oneWire);

String mon;
String di;
String tri;

int takvim[] = {0,1,23};
int trello[] = {20,21,22};
int hava[] = {2,3,4};
int twitter[] = {17,18,19};
int airq[] = {8,9,10};
int mail[] = {14,15,16};
int sicaklik[] = {5,6,7};
int su[] = {11,12,13};

boolean sifirla = false;
boolean pomodoro = false;
long buttonTimer = 0;
long longPressTime = 500;

boolean buttonActive = false;
boolean longPressActive = false;

BlynkTimer timer1;
BlynkTimer timer2;

WidgetRTC rtc;

void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  rtc.begin();
  DS18B20.begin();
  ring.setBrightness(BRIGHTNESS);
  ring.begin();
  ring.show();
  pinMode(BUTTON, INPUT);
  pinMode(BUZZER, OUTPUT);
  timer1.setInterval(1000L, havaKalitesi);
  timer2.setInterval(1000L, getTemperature);
}

void loop()
{
  Blynk.run();
  timer1.run();
  timer2.run();
  s_mode();
}

void getTemperature() 
{
    float temp;
    DS18B20.requestTemperatures(); 
    temp = DS18B20.getTempCByIndex(0);
    Serial.println(temp);
    if(temp > 0 && temp < 20)
    {
      //soğuk
      for(int i = 0; i <= 2; i++)
      {
        ring.setPixelColor(sicaklik[i], 0, 0 , 255);
        ring.show();
      }
    }
    else if(temp >= 20 && temp <= 25)
    {
      //iyi
      for(int i = 0; i <= 2; i++)
      {
        ring.setPixelColor(sicaklik[i], 0, 255 , 0);
        ring.show();
      }
    }
    else if(temp > 25)
    {
      //sıcak
      for(int i = 0; i <= 2; i++)
      {
        ring.setPixelColor(sicaklik[i], 255, 0 , 0);
        ring.show();
      }
    }  
}

void havaKalitesi()
{
  int hava = analogRead(A0);
  //Serial.println(hava);
  if(hava < 250)
  {
    //Serial.println("iyi");
    for(int i = 0; i <= 2; i++)
    {
      ring.setPixelColor(airq[i], 0, 255 , 0);
      ring.show();
    }
  }else if(hava > 250 && hava < 650)
  {
    //Serial.println("orta");
    for(int i = 0; i <= 2; i++)
    {
      ring.setPixelColor(airq[i], 255, 255 , 0);
      ring.show();
    }
  }
  else if(hava > 650)
  {
    //Serial.println("kotu");
    for(int i = 0; i <= 2; i++)
    {
      ring.setPixelColor(airq[i], 255, 0 , 0);
      ring.show();
    }
  }else
  {
    //Serial.println("sensor bozuk");
    for(int i = 0; i <= 2; i++)
    {
      ring.setPixelColor(airq[i], 0, 0 , 0);
      ring.show();
    }
  }
}

void sondur()
{
  for(int x = 0; x <= NUM_LEDS-1; x++)
  {
    ring.setPixelColor(x, 0, 0 , 0);
    ring.show();
  }
}

void bip()
{
  for(int i = 0; i < 3; i++)
  {
    digitalWrite(BUZZER, HIGH);
    delay(100);
    digitalWrite(BUZZER, LOW);
    delay(100);
  }
}

void say(int calis, int mola)
{
  sondur();
  bip();
  calis = calis * 60;
  for(int i = 0; i<= calis; i++)
  {
    delay(200);
    int yan = map(i, 0, calis, 0,NUM_LEDS);
    ring.setPixelColor(yan, 255, 0 , 0);
    ring.show();
    Serial.println(i);
  }
  bip();
  delay(1000);
  sondur();
  Serial.println("mola");
  mola = mola * 60;
  for(int i = 0; i<= mola; i++)
  {
    delay(200);
    int yan = map(i, 0, mola, 0,NUM_LEDS);
    ring.setPixelColor(yan, 0, 255 , 0);
    ring.show();
    Serial.println(i);
  }
  bip();
  delay(2000);
  sondur();
  Serial.println("bitti");
}

void s_mode()
{
  if (digitalRead(BUTTON) == HIGH)
  {
    if (buttonActive == false)
    {
      buttonActive = true;
      buttonTimer = millis();
    }
    if ((millis() - buttonTimer > longPressTime) && (longPressActive == false))
    {
      longPressActive = true;
      pomodoro = !pomodoro;
      if (pomodoro == true && sifirla == false)
      {
        Serial.println("Pomodoro");
        say(5,2);
        pomodoro = false;
      }
    }

  }
  else
  {
    if (buttonActive == true)
    {
      if (longPressActive == true)
      {
        longPressActive = false;
      }
      else
      {
        sifirla = !sifirla;
        if (sifirla == true && pomodoro == false)
        {
          Serial.println("sifirladim");
          sondur();
          sifirla = false;
        }
      }
      buttonActive = false;
    }
  }
}


BLYNK_WRITE(V0)
{
  mon = param[0].asStr();
  di = param[1].asStr();
  tri = param[2].asStr();
  Serial.println(mon);
  Serial.println(di);
  Serial.println(tri);
  
  if(mon == "mail")
  {
    for(int i = 0; i <= 2; i++)
    {
      ring.setPixelColor(mail[i], 255, 0 , 0);
      ring.show();
    }
    bip();
  }
  if(mon == "su")
  {
    for(int i = 0; i <= 2; i++)
    {
      ring.setPixelColor(su[i], 0, 0 , 255);
      ring.show();
    }
    bip();
  }
  if(mon == "twitter")
  {
    for(int i = 0; i <= 2; i++)
    {
      ring.setPixelColor(twitter[i], 0, 0 , 255);
      ring.show();
    }
    bip();
  }
  if(mon == "trello")
  {
    for(int i = 0; i <= 2; i++)
    {
      ring.setPixelColor(trello[i], 0, 0 , 255);
      ring.show();
    }
    bip();
  }
  if(mon == "takvim")
  {
    for(int i = 0; i <= 2; i++)
    {
      ring.setPixelColor(takvim[i], 255, 80 , 180);
      ring.show();
    }
    bip();
  }
  if(mon == "hava_sicaklik")
  {
    int dint = di.toInt();

    if(dint <= 16)
    {
      //soğuk
       for(int i = 0; i <= 2; i++)
      {
        ring.setPixelColor(hava[i], 0, 0 , 255);
        ring.show();
      }
    }
    else if(dint > 16 && dint <= 26)
    {
      //iyi
      for(int i = 0; i <= 2; i++)
      {
        ring.setPixelColor(hava[i], 255, 255 , 0);
        ring.show();
      }
    }
    else if(dint > 26)
    {
      //sıcak
      for(int i = 0; i <= 2; i++)
      {
        ring.setPixelColor(hava[i], 255, 0 , 0);
        ring.show();
      }
    }
  }
    
}


