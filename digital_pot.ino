const int up = 8;  // izpolzvame const int za read only variables
const int down =  7;  
int up_state = 0;
int down_state = 0;

const int inc =  10;    
const int ud =  9;   

int loop_cnt = 0;

void setup() {
  Serial.begin(9600);
  
  pinMode(up, INPUT_PULLUP); 
  pinMode(down, INPUT_PULLUP);
  pinMode(inc, OUTPUT);
  pinMode(ud, OUTPUT);
  digitalWrite(inc, HIGH);
  digitalWrite(ud, HIGH);
  
  while (!Serial) {
    delay(1);
  }

}

const int loopPeriod = 50;  //Loop period equals three times of this value, ex) 50 * 3 = 150ms

void loop() {

  loop_cnt = loop_cnt + 1;
  
    //Read the level of Up/Down button
  up_state = digitalRead(up);
  down_state = digitalRead(down);
  
    //Check if Up/Down Button is Pressed
    //When Button is Pressed, Level becomes LOW
  if (up_state == LOW){
    digitalWrite(ud, HIGH);
    delay(loopPeriod);
    digitalWrite(inc, LOW);
    delay(loopPeriod);
    digitalWrite(inc, HIGH);
    delay(loopPeriod);
  }else if (down_state == LOW){
    digitalWrite(ud, LOW);
    delay(loopPeriod);
    digitalWrite(inc, LOW);
    delay(loopPeriod);
    digitalWrite(inc, HIGH);
    delay(loopPeriod);
  }else{
      delay(loopPeriod*3);
   }

}
