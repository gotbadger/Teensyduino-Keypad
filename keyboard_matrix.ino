const int ledPin = 13;   // Teensy has LED on 13
const byte ROWS = 6; 
const byte COLS = 4; 
//                    //R1,R2...
//byte rowPins[ROWS] = {7, 8, 9, 10, 11, 12 };
//                    //C1,C2...
//byte colPins[COLS] = { 17, 16, 15, 14 }; 

byte rowPins[ROWS] = {7, 8, 9, 10, 11, 12};
                    //C1,C2...
byte colPins[COLS] = { 14, 15, 16, 17 }; 

void setup()   {                
  Serial.begin(38400);
  pinMode(ledPin, OUTPUT);
  //setup pins cols output
  for(int c=0; c<COLS; c++){
    pinMode(colPins[c], OUTPUT);
    digitalWrite(colPins[c], LOW);
  }
  for(int r=0; r<ROWS; r++){
    pinMode(rowPins[r], INPUT);
    //pull down using some teensy magix
    *portConfigRegister(rowPins[r]) = PORT_PCR_MUX(1) | PORT_PCR_PE;
  }
}

void loop()                     
{
  digitalWrite(ledPin, HIGH);
  //run though each col turining it on
  for(int c=0; c<COLS; c++){
    digitalWrite(colPins[c], HIGH);
    //see what happened when we turned this on
    for(int r=0; r<ROWS; r++){
      if(isRowActive(r)){
        //was high - generate activity 
        activity(c,r);
      }
    }
    digitalWrite(colPins[c], LOW);
  }
  //slow scan down for debug
  delay(200);  
  //Serial.println("Tick..");
}
void activity(byte col, byte row){
  Serial.print(col);
  Serial.print("x");
  Serial.println(row);
}
boolean isRowActive(int row){
  return digitalRead(rowPins[row]) == HIGH;
}
