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

#define CHARACTER_COUNT 53 //how many characters we can decode - used for array length

// Global variables
LiquidCrystal_I2C g_lcd(LCDADDRESS, LCDWIDTH, LCDHEIGHT);
boolean g_key_down;
boolean g_key_down_previous = false;
int     g_cursor_x = 0;
int     g_cursor_y = 0;
float   g_dash_length = 200.0;
long    g_signal_start_time;
long    g_silence_start_time;
String  g_current_char_code = "";
String  g_characters[CHARACTER_COUNT][2] = {
  {"A", ".-"}, {"B", "-..."}, {"C", "-.-."}, {"D", "-.."}, {"E", "."}, {"F", "..-."}, {"G", "--."},
  {"H", "...."}, {"I", ".."}, {"J", ".---"}, {"K", "-.-"}, {"L", ".-.."}, {"M", "--"}, {"N", "-."},
  {"O", "---"}, {"P", ".--."}, {"Q", "--.-"}, {"R", ".-."}, {"S", "..."}, {"T", "-"}, {"U", "..-"},
  {"V", "...-"}, {"W", ".--"}, {"X", "-..-"}, {"Y", "-.--"}, {"Z", "--.."},
  {"0", "-----"}, {"1", ".----"}, {"2", "..---"}, {"3", "...--"}, {"4", "....-"}, 
  {"5", "....."}, {"6", "-...."}, {"7", "--..."}, {"8", "---.."}, {"9", "----."},
  {".", ".-.-.-"},
  {",", "--..--"},
  {"?", "..--.."},
  {";", "-.-.-."},
  {":", "---..."},
  {"-", "-....-"},  //hyphen
  {"/", "-..-."},   //slash
  {"'", ".----."},  //single quote
  {"\"", ".-..-."}, //double quote
  {"_", "..--.-"},  //underscore
  {"+", ".-.-."},   //addition
  {"-", "-....-"},  //subtraction
  {"*", "-..-"},    //multiplication
  {"/", "---..."},  //division
  {"=", "-...-"},
  {")", "-.--.-"},
  {"(", "-.--."}
};

void setup() {
  g_lcd.begin();
  g_lcd.backlight();
  g_lcd.clear();
  g_lcd.setCursor(0,0);
  
  pinMode(KEY_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  
  Serial.begin(115200);
  print("Morse:");
}

void loop() {
  g_key_down = !digitalRead(KEY_PIN);

  if (g_key_down) {
    if (g_key_down != g_key_down_previous) {
      g_signal_start_time = millis();
      event_key_down(g_silence_start_time);
    }
    digitalWrite(LED_PIN, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);
  }
  else {
    if (g_key_down != g_key_down_previous) {
      g_silence_start_time = millis();
      event_key_up(g_signal_start_time);
    }
    digitalWrite(LED_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);
  }

  if (abs(millis() - g_silence_start_time) > g_dash_length * 10) {
    event_key_down(g_silence_start_time);
  }

  g_key_down_previous = g_key_down;
}

void event_key_up(long start_time) {
  char symbol = '?';
  long time = abs(millis() - start_time); // Duration of the signal (is dit or dah?)
  float dot_length = g_dash_length / 3.0;

  if (time <= 2) return;

  if (time <= dot_length) symbol = '.';
  else if (time > g_dash_length) symbol = '-';
  else if ((time > (g_dash_length + dot_length) / 1.9) && time <= g_dash_length) symbol = '-';
  else symbol = '.';

  if (symbol == '-') {
    if (time > g_dash_length) g_dash_length++;
    if (time < g_dash_length) g_dash_length--;
  }
  else if (symbol == '.') {
    if (time > g_dash_length / 3.0) g_dash_length++;
    if (time < g_dash_length / 3.0) g_dash_length--;
  }
  g_current_char_code += symbol;
}

void event_key_down(long start_time) {
  if (g_current_char_code == "")
    return;

  char symbol = '?';
  long time = abs(millis() - start_time);
  if (time > g_dash_length - g_dash_length / 40) {
    decode_character();
  }
  if (time > g_dash_length * 2) {
    decode_character();
    print(' ');
  }
}

void decode_character() {
  if (g_current_char_code == "")
    return;
    
  boolean found = false;
  for (int i = 0; i < CHARACTER_COUNT && !found; ++i) {
    if (g_characters[i][1] == g_current_char_code) {
      print(g_characters[i][0]);
      found = true;
    }
  }
  
  if (!found) {
    // if we can't decode it, output what we thought we heard for feedback.
    print(g_current_char_code);
  }
  g_current_char_code = "";
}

// Prints to the attached LCD screen
void print(char s) {
  Serial.print(s);
  if (g_cursor_x == 0 && g_cursor_y == 0) {
    g_lcd.clear();
  }
  g_lcd.setCursor(g_cursor_x, g_cursor_y);
  g_lcd.print(s);
  g_cursor_x++;
  if (g_cursor_x >= LCDWIDTH) {
    g_cursor_x = 0;
    g_cursor_y++;
  }
  if (g_cursor_y >= LCDHEIGHT) {
    g_cursor_x=g_cursor_y = 0;
  }
  // Print an underscore for the current cursor position
  if (!(g_cursor_x == 0 && g_cursor_y == 0)) {
    g_lcd.print('_');
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
