#include <LiquidCrystal.h>
LiquidCrystal lcd(3, 2, A3, A2, A1, A0);
const float V_REF = 5.0;     
int UP = 0;
int DOWN = 1;
int RST = 0;
const int UD_PIN = 8;       
const int INC_PIN = 9;    
const int CS_PIN = 10;      
const int WIPER_PIN = A5;     
float voltage = 0;
const int up_button = 7;
const int down_button = 6;
const int rst_button = 11;


byte RF[8] = {
  0b00001,
  0b00001,
  0b00001,
  0b00001,
  0b00001,
  0b00001,
  0b00001,
  0b11111
};
byte Mid[8] = {
  0b11111,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};
byte FF[8] = {
  0b10000,
  0b10000,
  0b10000,
  0b10000,
  0b10000,
  0b10000,
  0b10000,
  0b11111
};

void setup() {
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  digitalWrite(A0, HIGH);
  digitalWrite(A1, HIGH);
  digitalWrite(A2, HIGH);
  digitalWrite(A3, HIGH);
  lcd.createChar(1, RF);
  lcd.createChar(2, Mid);
  lcd.createChar(3, FF);
  lcd.begin(16, 2);
  lcd.clear();
  lcd.write(byte(1));
  lcd.write(byte(2));
  lcd.write(byte(3));
  lcd.write(byte(1));
  lcd.write(byte(2));
  lcd.write(byte(3));
  lcd.write(byte(1));
  lcd.write(byte(2));
  pinMode (CS_PIN, OUTPUT);
  pinMode (UD_PIN, OUTPUT);
  pinMode (INC_PIN, OUTPUT);
  pinMode(up_button, INPUT_PULLUP);  
  pinMode(down_button, INPUT_PULLUP);
  pinMode(rst_button, INPUT_PULLUP);
  Serial.begin(9600);

  digitalWrite(INC_PIN, HIGH);
  digitalWrite(UD_PIN, HIGH);
  digitalWrite(CS_PIN, LOW);
}

void loop() {
  if (Serial.available()) DoSerial(); 
  Serial.println(analogRead(A4));
  UP = digitalRead(up_button);
  DOWN = digitalRead(down_button);
  RST = digitalRead(rst_button);
  if (UP == LOW){
    digitalWrite(UD_PIN, HIGH);
    delay(50);
    digitalWrite(INC_PIN, LOW);
    delay(50);
    digitalWrite(INC_PIN, HIGH);
    delay(50);
  }else if (DOWN == LOW){
    digitalWrite(UD_PIN, LOW);
    delay(50);
    digitalWrite(INC_PIN, LOW);
    delay(50);
    digitalWrite(INC_PIN, HIGH);
    delay(50);
  }else if (RST == LOW){
    for(int i = 0; i <= 101; i++ ){
      digitalWrite(UD_PIN, LOW);
      digitalWrite(INC_PIN, LOW);
      digitalWrite(INC_PIN, HIGH);
    }
    for(int i = 0; i <= 50; i++){
      digitalWrite(UD_PIN, HIGH);
      digitalWrite(INC_PIN, LOW);
      digitalWrite(INC_PIN, HIGH);
    } 
    delay(100);
  }else{
    delay(50);
  }
}

void DoSerial()
{
  char ch = toupper(Serial.read());
  switch (ch) {
    case 'S':                         // Save settings
      digitalWrite(INC_PIN, HIGH);
      digitalWrite(CS_PIN, HIGH);
      delay(100);
      digitalWrite(CS_PIN, LOW);
      break;
    case 'U':                         // Increment setting
      Move_Wiper(0);
      break;
    case 'D':                         // Decrement setting
      Move_Wiper(1);
      break;
    default:
      break;
  }
}

void Move_Wiper(int direction)
{
  switch (direction) {
    case 0:
      digitalWrite(UD_PIN, HIGH); delayMicroseconds(5); 
      digitalWrite(INC_PIN, LOW); delayMicroseconds(5); 
      digitalWrite(INC_PIN, HIGH);
      break;
    case 1:
      digitalWrite(UD_PIN, LOW); delayMicroseconds(5); 
      digitalWrite(INC_PIN, LOW); delayMicroseconds(5); 
      digitalWrite(INC_PIN, HIGH);
      break;
    default:
      break;
  }
}
