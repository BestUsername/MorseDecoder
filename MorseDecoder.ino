/*
 * Morse-decoder
 * Based off Morse-decodeerder by Demented hacker in 2016
 * https://www.instructables.com/id/Arduino-Morse-Decoder/
 * (Translated by google)
 * Changed LCD to use 1602 using 
 * http://www.dreamdealer.nl/tutorials/connecting_a_1602a_lcd_display_and_a_light_sensor_to_arduino_uno.html
 */

#include <Wire.h>
#include <LiquidCrystal.h>

// Connect a tactile switch (or signal key) between pin 6 of the Arduino and GND.
// And connect a buzzer between Arduino pin 13 and GND.
// Then you can signal with the switch. View the decoded Morse code in the serial monitor.

#define KEY 6
#define BUZZER 13

#define LCDWIDTH 16 // 1602 is 16 characters wide
#define LCDHEIGHT 2 // 1602 is 02 characters tall

LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

void setup() {
  lcd.begin(LCDWIDTH, LCDHEIGHT);
  pinMode(KEY, INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT);
  Serial.begin(115200);
  print("Morse:");
}

float dash_length = 200.0;
boolean PrevS = false;
long tStartTeken, tStartPauze;
boolean S;
String kar = "";
int y = 0, x = 0;
void loop() {
  S = !digitalRead(KEY);

  if (S) {
    if (S != PrevS) {
      tStartTeken = millis();
      decoderPauze(tStartPauze);
    }
    digitalWrite(BUZZER, HIGH);
  }
  else {
    if (S != PrevS) {
      tStartPauze = millis();
      decoder(tStartTeken);
    }
    digitalWrite(BUZZER, LOW);
  }

  if (abs(millis() - tStartPauze) > dash_length * 10) {
    decoderPauze(tStartPauze);
  }

  PrevS = S;
}

void decoder(long start_time) {
  char symbol = '?';
  long time = abs(millis() - start_time); // Duration of the signal (a point or a line)
  float dot_length = dash_length / 3.0;

  if (time <= 2) return; // Denderonderdrukking

  if (time <= dot_length) symbol = '.';
  else if (time > dash_length) symbol = '-';
  else if ((time > (dash_length + dot_length) / 1.9) && time <= dash_length) symbol = '-';
  else symbol = '.';

  if (symbol == '-') {
    if (time > dash_length) dash_length++;
    if (time < dash_length) dash_length--;
  }
  else if (symbol == '.') {
    if (time > dash_length / 3.0) dash_length++;
    if (time < dash_length / 3.0) dash_length--;
  }
  kar += symbol;
  //Serial.println(symbol);
}

void decoderPauze(long start_time) {
  if (kar == "") return;

  char symbol = '?';
  long time = abs(millis() - start_time);
  if (time > dash_length - dash_length / 40) {
    decoderKar();
    //Serial.print();
  }
  if (time > dash_length * 2) {
    decoderKar();
    print(' ');
  }
}

void decoderKar() {
  static String letters[] = {
    ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-",
    ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..", "E"
  };
  int i = 0;
  while (letters[i] != "E") {
    if (letters[i] == kar) {
      print((char)('A' + i));
      break;
    }
    i++;
  }
  if (letters[i] == "E") {
    print(kar);
  }
  kar = "";
}

void blank() {
  lcd.clear();
}

void print(char s) {
  Serial.print(s);
  if (x == 0 && y == 0) {
    blank();
  }
  lcd.setCursor(x, y);
  lcd.print(s);
  x++;
  if (x >= LCDWIDTH) {
    x = 0;
    y++;
  }
  if (y >= LCDHEIGHT) {
    x=y = 0;
  }
  if (!(x == 0 && y == 0)) {
    lcd.print('_');
  }
}
void print(String &s) {
  for (int i = 0; i < s.length(); i++) {
    print(s[i]);
  }
}
void print(const char *s) {
  for (int i = 0; i < strlen(s); i++) {
    print(s[i]);
  }
}
