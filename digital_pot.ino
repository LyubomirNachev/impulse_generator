const float V_REF = 5.0;     
const int UP = 0;
const int DOWN = 1;
const int UD_PIN = 8;       
const int INC_PIN = 9;    
const int CS_PIN = 10;      
const int WIPER_PIN = A5;     
float voltage = 0;

void setup() {
  pinMode (CS_PIN, OUTPUT);
  pinMode (UD_PIN, OUTPUT);
  pinMode (INC_PIN, OUTPUT);
  Serial.begin(9600);

  digitalWrite(INC_PIN, HIGH);
  digitalWrite(CS_PIN, LOW);                  // Enable the X9C103P chip
  //Serial.print ("Initial Voltage Setting: ");
  PrintVoltage();                             // Print X9C103P power up value
}

void loop() {
  if (Serial.available()) DoSerial();  // loop looking for user input
  Serial.println(analogRead(A4));
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
      Move_Wiper(UP);
      //Serial.print("Incrementing Value");
      PrintVoltage();
      break;
    case 'D':                         // Decrement setting
      Move_Wiper(DOWN);
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
    case UP:
      digitalWrite(UD_PIN, HIGH); delayMicroseconds(5); 
      digitalWrite(INC_PIN, LOW); delayMicroseconds(5); 
      digitalWrite(INC_PIN, HIGH);
      break;
    case DOWN:
      digitalWrite(UD_PIN, LOW); delayMicroseconds(5); 
      digitalWrite(INC_PIN, LOW); delayMicroseconds(5); 
      digitalWrite(INC_PIN, HIGH);
      break;
    default:
      break;
  }
}
