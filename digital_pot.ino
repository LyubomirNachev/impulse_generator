#include <LiquidCrystal.h>
LiquidCrystal lcd(3, 2, A3, A2, A1, A0);
const float V_REF = 5.0;     
int UP = 0;
int DOWN = 1;
int RST = 0;
const int UD_PIN = 4;       
const int INC_PIN = 9;    
const int CS_PIN = 10;      
const int WIPER_PIN = A5;     
float voltage = 0;
const int up_button = 7;
const int down_button = 6;
const int rst_button = 11;

volatile unsigned int C1 = 0, C2 = 0, sumDiff = 0, capNum, numCaptures=0;
volatile bool state = false, timer1CountDone = false; float f = 0;

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

  analogReference(INTERNAL);
  
  //Timer 1: input capture interrupt (pin D8) & overflow interrupt
  noInterrupts();
  TCCR1A = 0; TCCR1B = 0; //reset control registers
  TCNT1 = 0;              //reset counter register
  TCCR1B |= 0b11000100;   //256 prescaler, rising edge, noise canceler
  TIMSK1 |= 0b00100001;   //enable input capture & overflow interrupts
  interrupts();
}


ISR(TIMER1_CAPT_vect)
{
  capNum++;
  if(capNum == 1) C1 = ICR1;
  if(capNum == 2)
  {
    C2 = ICR1;
    sumDiff += (C2 - C1);
    numCaptures++;
    C1 = 0; C2 = 0; capNum = 0;
  }
}
//-----------------------------
ISR(TIMER1_OVF_vect)
{
  C1 = 0; C2 = 0; capNum = 0;
  timer1CountDone = true;
  state = !state;
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
  while(!timer1CountDone) {}
  //--------------------------------------------------
  f = 1/(float(sumDiff/numCaptures)*16.0e-6);
  numCaptures = 0;
  sumDiff = 0;
  timer1CountDone = false;
  lcd.setCursor(0,1);
  lcd.print("Freq: ");
  lcd.print(f);
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
