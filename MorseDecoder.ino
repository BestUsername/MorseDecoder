/*
 * Morse-decoder
 * 
 * Based off Morse-decodeerder by Demented hacker in 2016
 * 1) https://www.instructables.com/id/Arduino-Morse-Decoder/
 * 
 * Changed LCD to use 1602 using 
 * 2) http://www.dreamdealer.nl/tutorials/connecting_a_1602a_lcd_display_and_a_light_sensor_to_arduino_uno.html
 * 
 * Changed to I2C using (includes information to install library and find the I2C address for your adapter)
 * 3) https://create.arduino.cc/projecthub/akshayjoseph666/interface-i2c-16x2-lcd-with-arduino-uno-just-4-wires-273b24
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Connect a tactile switch (or signal key) between pin 5 of the Arduino and GND.
// Connect a LED between pin 6 and GND (or change the pin to LED_BUILTIN or 13 to use the on-board LED)
// Connect a buzzer between Arduino pin 7 and GND.
// Connect the 4 pins of the LCD to GND(GND), 5V(VCC), A4(SDA) and A5(SCL)
// Then you can signal with the switch. View the decoded Morse code in the serial monitor.

#define KEY_PIN 5
#define LED_PIN 6
#define BUZZER_PIN 7


#define LCDADDRESS 0x27 // default address - use link 3) in the header to detect the address
#define LCDWIDTH 16 // 1602 is 16 characters wide
#define LCDHEIGHT 2 // 1602 is 02 characters tall

// Global variables
LiquidCrystal_I2C lcd(LCDADDRESS, LCDWIDTH, LCDHEIGHT);
float dash_length = 200.0;
boolean PrevS = false;
long tStartTeken, tStartPauze;
boolean S;
String kar = "";
int y = 0, x = 0;

void setup() {
  lcd.begin();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0);
  
  pinMode(KEY_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  
  Serial.begin(115200);
  print("Morse:");
}

void loop() {
  S = !digitalRead(KEY_PIN);

  if (S) {
    if (S != PrevS) {
      tStartTeken = millis();
      decoderPauze(tStartPauze);
    }
    digitalWrite(LED_PIN, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);
  }
  else {
    if (S != PrevS) {
      tStartPauze = millis();
      decoder(tStartTeken);
    }
    digitalWrite(LED_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);
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
  // A-Z
  static String letters[] = {
    ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-",
    ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..", "E"
  };

  // 0-9
  static String numbers[] = {
    "-----", ".----", "..---", "...--", "....-", ".....", "-....", "--...", "---..", "----."
  };

  static String punctuation[] = {
    ".-.-.-", // .
    "--..--", // ,
    "..--..", // ?
    "-.-.-.", // ;
    "---...", // :
    "-....-", // - (hyphen)
    "-..-.",  // / (slash)
    ".----.", // '
    ".-..-."  // "
  };

  static String special[] = {
    "..--.-", // _ (underscore)
    ".-.-.",  // + (addition)
    "-....-", // - (subtraction)
    "-..-",   // * (multiplication)
    "---...", // / (division)
    "-...-",  // = (equals)
    "-.--.-", // )
    "-.--."   // (
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

void print(char s) {
  Serial.print(s);
  if (x == 0 && y == 0) {
    lcd.clear();
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
