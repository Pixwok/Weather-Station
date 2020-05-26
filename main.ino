
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

/*Capteur*/
#define DHTPIN 2
#define DHTTYPE DHT11
DHT_Unified dht(DHTPIN, DHTTYPE);

/*LCD*/
LiquidCrystal_I2C lcd(0x27,20,4);

/*VARIABLE*/
float t;
float h;
unsigned long StartTime, previousTime;
unsigned long cooldown = 20000; //Mesure toutes les 30 secondes
int light;

void setup()
{
  Serial.begin(9600);
  dht.begin();
  lcd.init();
  lcd.backlight();
  lcd.setCursor(7,1);
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  t = event.temperature;
  dht.humidity().getEvent(&event);
  h = event.relative_humidity;
  light = analogRead(A0);
}
float measure()
{
  StartTime = millis();
  if((StartTime - previousTime) > cooldown)
  {
    previousTime = StartTime;
    sensors_event_t event;
    dht.temperature().getEvent(&event);
    t = event.temperature;
    dht.humidity().getEvent(&event);
    h = event.relative_humidity;
    light = analogRead(A0);
  }
}

void loop()
{
  measure();
  lcd.setCursor(0,1);
  lcd.print("T="); lcd.print(t, 1); lcd.print(" Degres");
  lcd.setCursor(0,2);
  lcd.print("H="); lcd.print(h, 0); lcd.print("%");  
  lcd.setCursor(0,3);
  lcd.print("Eclairage:"); lcd.print(light);
  
  if (StartTime >= 2592000000 ) //RESET DE LA CARTE AU BOUT DE 30 JOURS
  {
    AutoReset(); //APPEL FONCTION AUTO RESET
  }
}

//UTILISATION D'UNE RESISTANCE DE 1K OHM ENTRE LES BORNES 9 ET RESET
void AutoReset()
{
  int ResetPIN = 9;
  pinMode(ResetPIN, OUTPUT);
  digitalWrite(ResetPIN, LOW);
}
