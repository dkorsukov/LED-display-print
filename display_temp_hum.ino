#include <Adafruit_Sensor.h>
#include <DHT.h>;
#include <DHT_U.h>;

// time stamps
int lastBtnCheck = 0;
int lastPrint = 0;
int lastDHTRead = 0;

// btn flag
boolean mode = 0;

// pins
const byte digitsPins[] = { A0, A1, A2, A3 }; // for display pins 12, 9, 8, 6
const byte segmentsPins[] = { 2, 3, 4, 5, 6, 7, 8, 9 }; // for display pins 1, 2, 3, 4, 5, 7, 10, 11
const byte btnPin = 12;

// display symbols
const boolean symbols[][ sizeof(segmentsPins) ] = {
  { 1, 1, 0, 1, 0, 1, 1, 1 }, // 0
  { 0, 0, 0, 1, 0, 1, 0, 0 }, // 1
  { 1, 1, 0, 0, 1, 1, 0, 1 }, // 2
  { 0, 1, 0, 1, 1, 1, 0, 1 }, // 3
  { 0, 0, 0, 1, 1, 1, 1, 0 }, // 4
  { 0, 1, 0, 1, 1, 0, 1, 1 }, // 5
  { 1, 1, 0, 1, 1, 0, 1, 1 }, // 6
  { 0, 0, 0, 1, 0, 1, 0, 1 }, // 7
  { 1, 1, 0, 1, 1, 1, 1, 1 }, // 8
  { 0, 1, 0, 1, 1, 1, 1, 1 }, // 9
  { 1, 0, 0, 1, 1, 1, 1, 1 }  // A
};

DHT dht(11, DHT11);

float dhtValue = 0;

void displayPrint(String str) {
  String forPrint = str.substring(0, 4);
  
  for (byte symbolIndex = 0; symbolIndex < forPrint.length(); symbolIndex++) {
    byte symbolInIndex;
    
    if ( isDigit(forPrint[symbolIndex]) ) {
      symbolInIndex = byte(forPrint[symbolIndex]) - 48;
    } else if ( isAlpha(forPrint[symbolIndex]) && isUpperCase(forPrint[symbolIndex]) ) {
      symbolInIndex = byte(forPrint[symbolIndex]) - 55;
    } else {
      continue;
    }
      
    for ( byte segmentIndex = 0; segmentIndex < sizeof( symbols[symbolInIndex] ); segmentIndex++ ) {
      digitalWrite(segmentsPins[segmentIndex], symbols[symbolInIndex][segmentIndex]);
    }

    boolean withPoint = byte( forPrint[symbolIndex + 1] ) == 46;

    Serial.println(withPoint);
    
    if (withPoint) {
      digitalWrite(segmentsPins[2], HIGH);
    }
      
    digitalWrite(digitsPins[symbolIndex], LOW);
    delay(1);
    digitalWrite(digitsPins[symbolIndex], HIGH);

    if (withPoint) {
     digitalWrite(segmentsPins[2], LOW);

     forPrint.remove(symbolIndex + 1, 1);
    }
  }
}

void readDHTData() {
  int ms = millis();
  
  if (ms - lastDHTRead > 1000) {
    if (mode == 1) {
      dhtValue = dht.readTemperature();
    } else {
      dhtValue = dht.readHumidity();
    }

    lastDHTRead = ms;
  }
}

void setup() {
  dht.begin();
  
  pinMode(btnPin, INPUT);

  for (byte i = 0; i < sizeof(digitsPins); i++) {
    pinMode(digitsPins[i], OUTPUT);

    digitalWrite(digitsPins[i], HIGH);
  }

  for (byte i = 0; i < sizeof(segmentsPins); i++) {
    pinMode(segmentsPins[i], OUTPUT);
  }  
}

void loop() {
  int ms = millis();
  
  if ( ms - lastBtnCheck > 250 && digitalRead(btnPin) == HIGH ) {
    mode = !mode;

    Serial.println(mode);

    lastBtnCheck = ms;
  }

  readDHTData();

  if (dhtValue != 0) {
    displayPrint( String(dhtValue) );
  }
}
