const float V_REF = 5.0;     
int UP = 0;
int DOWN = 1;
const int UD_PIN = 8;       
const int INC_PIN = 9;    
const int CS_PIN = 10;      
const int WIPER_PIN = A5;     
float voltage = 0;
const int up_button = 7;
const int down_button = 6;

void setup() {
  pinMode (CS_PIN, OUTPUT);
  pinMode (UD_PIN, OUTPUT);
  pinMode (INC_PIN, OUTPUT);
  pinMode(up_button, INPUT_PULLUP);  
  pinMode(down_button, INPUT_PULLUP);
  Serial.begin(9600);

  digitalWrite(INC_PIN, HIGH);
  digitalWrite(UD_PIN, HIGH);
  digitalWrite(CS_PIN, LOW);                  // Enable the X9C103P chip
  //Serial.print ("Initial Voltage Setting: ");
  PrintVoltage();  
  for(int i = 0; i <= 101; i++ ){
    digitalWrite(UD_PIN, LOW);
    delay(50);
    digitalWrite(INC_PIN, LOW);
    delay(50);
    digitalWrite(INC_PIN, HIGH);
    delay(50);
  }
  for(int i = 0; i <= 50; i++){
    digitalWrite(UD_PIN, HIGH);
    delay(50);
    digitalWrite(INC_PIN, LOW);
    delay(50);
    digitalWrite(INC_PIN, HIGH);
    delay(50);  
  }
}

void loop() {
  if (Serial.available()) DoSerial();  // loop looking for user input
  Serial.println(analogRead(A4));
  UP = digitalRead(up_button);
  DOWN = digitalRead(down_button);
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
  }else{
    delay(50);
  }
}

void DoSerial()
{
  char ch = toupper(Serial.read());   // Read the character we received
  // and convert to upper case
  switch (ch) {
    case 'S':                         // Save settings
      digitalWrite(INC_PIN, HIGH);
      digitalWrite(CS_PIN, HIGH);
      delay(100);
      digitalWrite(CS_PIN, LOW);
      //Serial.println("Setting Saved");
      break;
    case 'U':                         // Increment setting
      Move_Wiper(0);
      //Serial.print("Incrementing Value");
      PrintVoltage();
      break;
    case 'D':                         // Decrement setting
      Move_Wiper(1);
      //Serial.print("Decrementing Value");
      PrintVoltage();
      break;
    default:
      break;
  }
}
void PrintVoltage()
{
  int sampleADC = analogRead(WIPER_PIN);      // Take reading on wiper pin
  float volts = (sampleADC * V_REF) / 1023.0; // Convert to voltage
  //Serial.print("   ADC = ");
  //Serial.print(sampleADC);
  //Serial.print("  Voltage = ");
  //Serial.println(volts, 3);
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
