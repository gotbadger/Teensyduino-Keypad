const int ledPin = 13;   // Teensy has LED on 13
const byte ROWS = 6; 
const byte COLS = 4;
const int BUFFSIZE = 6;
                   //R1,R2...
const byte rowPins[ROWS] = {7, 8, 9, 10, 11, 12};
                    //C1,C2...
const byte colPins[COLS] = {  17, 16, 15, 14  };

//buffer we will use for 6kro
int sendBuffer[BUFFSIZE] = {0,0,0,0,0,0};
int sendBufferPos = 0;

const byte keymap[ROWS][COLS] = {
  {KEY_A,KEY_B,KEY_C,KEY_D},
  {KEYPAD_1,KEYPAD_SLASH,KEYPAD_ASTERIX,KEYPAD_MINUS},
  {KEYPAD_7,KEYPAD_8    ,KEYPAD_8      ,KEYPAD_PLUS},
  {KEYPAD_4,KEYPAD_5    ,KEYPAD_6      ,KEY_INSERT},
  {KEYPAD_1,KEYPAD_2    ,KEYPAD_3      ,0},
  {0       ,KEYPAD_0    ,KEYPAD_PERIOD ,KEYPAD_ENTER}
};

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
  processBuffer();
}
void activity(byte col, byte row){
  Serial.print(col);
  Serial.print("x");
  Serial.print(row);
  Serial.print(" --> ");
  Serial.println(keymap[row][col]);
  bufferKey(keymap[row][col]);
}
boolean isRowActive(int row){
  return digitalRead(rowPins[row]) == HIGH;
}

//buffer control
void processBuffer(){
  Keyboard.set_key1(sendBuffer[0]);
  Keyboard.set_key2(sendBuffer[1]);
  Keyboard.set_key3(sendBuffer[2]);
  Keyboard.set_key4(sendBuffer[3]);
  Keyboard.set_key5(sendBuffer[4]);
  Keyboard.set_key6(sendBuffer[5]);
  Keyboard.send_now();
  //reset buffer
  for(int i=0;i<BUFFSIZE;i++){
    sendBuffer[i] = 0;
  }
  sendBufferPos = 0;
  //prevent spam
  delay(50); 
}
void bufferKey(byte key){
  //if buffer is full bad luck ignore this one
  if(sendBufferPos < BUFFSIZE){
    sendBuffer[sendBufferPos] = key;
    sendBufferPos++;
    Serial.println(sendBufferPos);
  }
}
