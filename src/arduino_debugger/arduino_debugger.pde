

/*

  Arduino Debugger. Zak Henry October 2011

  Message looks like {12:on} or {a1:off}
  Returns [timestamp in us]{pin:value}{pinb:valueb}{}....

*/

int pinStatus[20];
boolean sendData = false;
boolean serialFree = false;


void setup(){
  
  Serial.begin(9600);
  
  serialFree = true;
  
  Serial.println("ready");
  
//    pinStatus[15] = 1;
//   pinStatus[14] = 1;

  pinMode(13, OUTPUT);
  
  for (int i=0; i<20; i++){
//    pinStatus[i] = 1;
   
   if (i<13){
     pinMode(i, INPUT);
   }
  }
}

void loop(){
  
  if (serialFree){ //serial reads and writes are disabled when changing baud rate
    
    while (Serial.available()) {
      
      readCommand();
       
    }
    
    if (sendData){
      printPins(pinStatus);
    }
    
  }
  
  
  
  
}

void printPins(int pinArray[]){
  Serial.print("[");
  Serial.print(micros());
  Serial.print("]");
  
  for (int i=0; i<20; i++){
    
    if(pinArray[i]==1){
      Serial.print("{");
//      if (i>13){
//        Serial.print("a");
//        Serial.print(i-14);
//      }else{
//        Serial.print(i);
//      }

      Serial.print(i);
      
      Serial.print(":");
      
      if (i<14){
        Serial.print(digitalRead(i));
      }else{
        Serial.print(analogRead(i-14));
      }
      
      Serial.print("}");
    }
     
  }
  
  Serial.print("[");
  Serial.print(micros());
  Serial.print("]");
  
  Serial.println();
}

void processCommand(String command, String value){
  
//  Serial.print("command: ");
//  Serial.print(command);
//  Serial.print(", value: ");
//  Serial.println(value);
  
  if (command=="baud"){
    
    Serial.println("Ending communication on serial");
    Serial.print("Changing to ");
    Serial.print(Str2long(value));
    Serial.println(" baud");
    serialFree = false;
    Serial.flush();
    Serial.end();
    Serial.begin(Str2long(value));
    Serial.flush();
    
    Serial.print("New serial connection at ");
    Serial.print(Str2long(value));
    Serial.println(" baud");
    serialFree = true;
    
    
  }else if (command=="challenge"){
    
    Serial.println("{response:ok}");
    
  }else if (command=="print"){
    
    sendData = value=="begin";
    
  }else if (command=="pin_on"||command=="pin_off"){
    int pin = -1;
    
    if(value.charAt(0)=='a'){
      pin = Str2int(value.substring(1)); //trim off the 'a'
      pin += 14;
    }else{
      pin = Str2int(value);
    }
    
    if (value!=0&&value!=1){ //dont attempt to poll tx or rx pins
      if (command=="pin_on"){
        pinStatus[pin] = 1;
      }else if(command == "pin_off"){
        pinStatus[pin] = 0;
      }
    }
  }else if (command=="led"){
    
    if(value=="on"){
      digitalWrite(13, HIGH);
    }else{
      digitalWrite(13, LOW);
    }
    
  }else{
//    Serial.println("Unknown command");
    Serial.print("{");
    Serial.print(command);
    Serial.print(":");
    Serial.print(value);
    Serial.println("}");
  }

  
  
}

void readCommand(){
  
    String command, value;
    
    boolean messageStarted = false;
    boolean readingCommand = true;
        
    while(1){ //cycle until exit condition met
    
        /*Read a byte as it comes into the serial buffer*/
        char byteBuffer = Serial.read();

        if(byteBuffer > -1){ //Is the data a valid character?
        
        
          if (byteBuffer == '}') break; //end of message
          if (byteBuffer == ':') readingCommand = false; //changes over to reading the value
        
          if(messageStarted&&byteBuffer!=':'){ //Is the character not the beginning of a message?
            if (readingCommand){
              command+=byteBuffer;
            }else{
              value+=byteBuffer;
            }
           
          }
        
          if (byteBuffer == '{') messageStarted = true; //end char  
            
        }
    }
    
    processCommand(command, value);


}

long Str2long (String str){
  char this_char[str.length() + 1];
  str.toCharArray(this_char, sizeof(this_char));
  return atol(this_char);
}

int Str2int (String str){
  char this_char[str.length() + 1];
  str.toCharArray(this_char, sizeof(this_char));
  return atoi(this_char);
}
