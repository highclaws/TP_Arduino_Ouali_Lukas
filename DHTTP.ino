// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor

#include "DHT.h"

#define DHTPIN 2     // Digital pin connected to the DHT sensor
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);

float* hTab = new float[10];
float* tTab = new float[10];
float* fTab = new float[10];
int i = 0;

void setup() {
  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D5, OUTPUT);

  dht.begin();
}

void loop() {
  // Wait a few seconds between measurements.
  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  int j = 0;
  float hMoy = 0;
  float tMoy = 0;
  float fMoy = 0;

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  } else {
    hTab[i%10] = h;
    tTab[i%10] = t;
    fTab[i%10] = f;
    if(i > 9) {
      
      for( j = 0; j < 10; j++) {
        hMoy += hTab[j];
        tMoy += tTab[j];
        fMoy += fTab[j];
      }
      Serial.print(F("Moyenne de l'humidité "));
      Serial.print(hMoy / 10);
      Serial.print(F("\n Moyenne de la température "));
      Serial.print(tMoy / 10);
      Serial.print(F("\n Moyenne de la heat index "));
      Serial.print(fMoy / 10);
      Serial.println(F(""));
      
      // Serial.println(i); index i
      float medianeH = trie(hTab);
       float medianeT = trie(tTab);
       float medianeHF = trie(fTab);

      Serial.print(F("Mediane de l'humidité "));
      Serial.println(medianeH);
      Serial.print(F("Mediane de la température "));
      Serial.println(medianeT);
      Serial.print(F("Mediane de heat index "));
      Serial.println(medianeHF);
    }
    i++;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);
  Serial.print(F("Humidity: "));
  Serial.print(h);

  if(h > hTab[(i-1) % 10]) {
      Serial.println(F(" valeur en augmentation\n"));
      digitalWrite(D1, HIGH);   // turn the LED on (HIGH is the voltage level)
      delay(1000); // attendre 1 seconde
      digitalWrite(D1, LOW);    // turn the LED off by making the voltage LOW

   } else if(h == hTab[(i-1) % 10] )  {
      Serial.println(F(" valeur egale\n"));
      digitalWrite(D2, HIGH);   // turn the LED on (HIGH is the voltage level)
      delay(1000); // attendre 1 seconde
      digitalWrite(D2, LOW);    // turn the LED off by making the voltage LOW

   } else {
      Serial.println(F(" valeur en baisse\n"));
      digitalWrite(D5, HIGH);    // turn the LED off by making the voltage LOW
      delay(1000); // attendre 1 seconde
      digitalWrite(D5, LOW);    // turn the LED off by making the voltage LOW

   }

  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  if(t > tTab[(i-1) % 10]) {
    Serial.println(F(" valeur en augmentation\n"));
  } else if(t == tTab[(i-1) % 10] )  {
    Serial.println(F(" valeur egale\n"));
  } else {
    Serial.println(F(" valeur en baisse\n"));
  }
  Serial.print(f);
  Serial.print(F("°F"));
  if(f > fTab[(i-1) % 10]) {
    Serial.println(F(" valeur en augmentation\n"));
  } else if(f == fTab[(i-1) % 10] )  {
    Serial.println(F(" valeur egale\n"));
  } else {
    Serial.println(F(" valeur en baisse\n"));
  }
  Serial.print(F("Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));
}

float trie(float* tab) {  
      float* newTab = tab;
      for(i = 0; i < 10; i ++) {
        float tmp = tab[i];
        if(i == 0) {
          newTab[i] = tmp;
        } else {
          int j;
          for(j = 0; j < i - 1; j++) {
            if(newTab[j] > tmp) {
              float tmp2 = newTab[j];
              newTab[j] = tmp;
              tmp = tmp2;
            }
          }
          newTab[i] = tmp;
        }
      }
      return newTab[5];
}
