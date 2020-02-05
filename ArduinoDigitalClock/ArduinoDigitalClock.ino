boolean toggle = 0;
//the pins of 4-digit 7-segment display attach to pin2-13 respectively 
int a = 12;
int b = 13;
int c = 2;
int d = 3;
int e = 4;
int f = 6;
int g = 5;
int LED = 7;
int d4 = A3;
int d3 = A2;
int d2 = A1;
int d1 = A0;

//Button pin
int btn_Select = 9;
int btn_Up  = 10;
int btn_Down  = 11;

int select, up, down;
int lastSelect = 0, lastUp = 0, lastDown =0;
int stateSelect=0;
int stateStopwatch=0;
boolean stateUp = 0, stateDown = 0;

//Counter via memori
long int counter = 0;
long int counter2 = 0;
void setup()
{
  Serial.begin(9600);
  //set all the pins of the LED display as output
  pinMode(d1, OUTPUT);
  pinMode(d2, OUTPUT);
  pinMode(d3, OUTPUT);
  pinMode(d4, OUTPUT);
  pinMode(a, OUTPUT);
  pinMode(b, OUTPUT);
  pinMode(c, OUTPUT);
  pinMode(d, OUTPUT);
  pinMode(e, OUTPUT);
  pinMode(f, OUTPUT);
  pinMode(g, OUTPUT);
  pinMode(LED, OUTPUT);
  pinMode(btn_Select, INPUT_PULLUP);
  pinMode(btn_Up, INPUT_PULLUP);
  pinMode(btn_Down, INPUT_PULLUP);

  cli();//stop interrupts

//set timer1 interrupt
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = 15624;// = (16*10^6) / (1*1024) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS12 and CS10 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);  
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);

sei();//allow interrupts

}//end setup
ISR(TIMER1_COMPA_vect){//timer1 interrupt 1Hz toggles pin 13 (LED)
//generates pulse wave of frequency 1Hz/2 = 0.5kHz (takes two cycles for full wave- toggle high then toggle low)
  if (toggle){
    digitalWrite(LED,LOW);
    counter++;
    Serial.println(counter);
    toggle = 0;
      if (stateStopwatch == 1){
        counter2++;
      }
  }
  else{
    digitalWrite(LED,HIGH);
    counter++;
    Serial.println(counter);
    toggle = 1;
    if (stateStopwatch == 1){
        counter2++;
      }
  }
}
void loop(){
  //84600 == total counter for 24 hours
  if (counter >= 86400) {
    counter = counter - 86400;
  }
  //to avoid negative counter
  if (counter < 0) {
    counter = counter + 86400;
  }
  select = digitalRead(btn_Select);
  up = digitalRead(btn_Up);
  down = digitalRead(btn_Down);

  if (select!= lastSelect) { 
      if(select==LOW){
        stateSelect++;
        Serial.println("Masuk ke menu");
        Serial.print("stateSelect : ");
        Serial.println(stateSelect);
      } else {
        Serial.println("Keluar");
      }
   }
   lastSelect=select;
   
  if(stateSelect==0){
        hitung_jam(counter, 2);
      } 
      else if(stateSelect==1){
        hitung_jam(counter, 0);
                if (up!= lastUp) { 
                  if(up==LOW){
                    Serial.println("Up On");
                    counter= counter+3600;
                    } else {Serial.println("Up Off");
                  }
                }
                lastUp=up;
        
                if (down!= lastDown) { 
                  if(down==LOW){
                    Serial.println("Down On");
                    counter= counter-3600;
                    } else {Serial.println("Down Off");
                  }
                }
                lastDown=down;
                
      }
      else if(stateSelect==2){
                counter2=0;
        hitung_jam(counter, 1);
                if (up!= lastUp) { 
                          if(up==LOW){
                            Serial.println("Up On");
                            counter= counter+60;
                            } else {Serial.println("Up Off");
                          }
                        }
                 lastUp=up;
                 
                 if (down!= lastDown) { 
                  if(down==LOW){
                    Serial.println("Down On");
                    counter= counter-60;
                    } else {Serial.println("Down Off");
                  }
                }
                lastDown=down;
      }else if(stateSelect==3){
         stopwatch (counter2);
         stateStopwatch = 1;
                         if (up!= lastUp) { 
                          if(up==LOW){
                            Serial.println("Up On");
                            counter2= 0;
                            } else {Serial.println("Up Off");
                          }
                        }
                 lastUp=up;
                 
                 if (down!= lastDown) { 
                  if(down==LOW){
                    Serial.println("Down On");
                    counter2= 0;;
                    } else {Serial.println("Down Off");
                  }
                }
                lastDown=down;
      }
      else {
        stateSelect = 0;
      }
}//void loop end
   
void pickDigit(int x) //light up a 7-segment display
{
  //The 7-segment LED display is a common-anode one. So also use digitalWrite to  set d1 as LOW and the LED will go out
  digitalWrite(d1, LOW);
  digitalWrite(d2, LOW);
  digitalWrite(d3, LOW);
  digitalWrite(d4, LOW);

  switch(x)
  {
    case 0: 
    digitalWrite(d1, HIGH);//Light d1 up 
    break;
    case 1: 
    digitalWrite(d2, HIGH); //Light d2 up 
    break;
    case 2: 
    digitalWrite(d3, HIGH); //Light d3 up 
    break;
    default: 
    digitalWrite(d4, HIGH); //Light d4 up 
    break;
  }
}
//The function is to control the 7-segment LED display to display numbers. Here x is the number to be displayed. It is an integer from 0 to 9 
void pickNumber(int x)
{
  switch(x)
  {
    default: 
    zero(); 
    break;
    case 1: 
    one(); 
    break;
    case 2: 
    two(); 
    break;
    case 3: 
    three(); 
    break;
    case 4: 
    four(); 
    break;
    case 5: 
    five(); 
    break;
    case 6: 
    six(); 
    break;
    case 7: 
    seven(); 
    break;
    case 8: 
    eight(); 
    break;
    case 9: 
    nine(); 
    break;
  }
} 

void clearLEDs() //clear the 7-segment display screen
{
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
  
}

void zero() //the 7-segment led display 0
{
  digitalWrite(a, LOW);
  digitalWrite(b, LOW);
  digitalWrite(c, LOW);
  digitalWrite(d, LOW);
  digitalWrite(e, LOW);
  digitalWrite(f, LOW);
  digitalWrite(g, HIGH);
  
}

void one() //the 7-segment led display 1
{
  digitalWrite(a, HIGH);
  digitalWrite(b, LOW);
  digitalWrite(c, LOW);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
  
}

void two() //the 7-segment led display 2
{
  digitalWrite(a, LOW);
  digitalWrite(b, LOW);
  digitalWrite(c, HIGH);
  digitalWrite(d, LOW);
  digitalWrite(e, LOW);
  digitalWrite(f, HIGH);
  digitalWrite(g, LOW);
  
}
void three() //the 7-segment led display 3
{
  digitalWrite(a, LOW);
  digitalWrite(b, LOW);
  digitalWrite(c, LOW);
  digitalWrite(d, LOW);
  digitalWrite(e, HIGH);
  digitalWrite(f, HIGH);
  digitalWrite(g, LOW);
  
}

void four() //the 7-segment led display 4
{
  digitalWrite(a, HIGH);
  digitalWrite(b, LOW);
  digitalWrite(c, LOW);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, LOW);
  digitalWrite(g, LOW);
  
}

void five() //the 7-segment led display 5
{
  digitalWrite(a, LOW);
  digitalWrite(b, HIGH);
  digitalWrite(c, LOW);
  digitalWrite(d, LOW);
  digitalWrite(e, HIGH);
  digitalWrite(f, LOW);
  digitalWrite(g, LOW);
  
}

void six() //the 7-segment led display 6
{
  digitalWrite(a, LOW);
  digitalWrite(b, HIGH);
  digitalWrite(c, LOW);
  digitalWrite(d, LOW);
  digitalWrite(e, LOW);
  digitalWrite(f, LOW);
  digitalWrite(g, LOW);
  
}

void seven() //the 7-segment led display 7
{
  digitalWrite(a, LOW);
  digitalWrite(b, LOW);
  digitalWrite(c, LOW);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
  
}

void eight() //the 7-segment led display 8
{
  digitalWrite(a, LOW);
  digitalWrite(b, LOW);
  digitalWrite(c, LOW);
  digitalWrite(d, LOW);
  digitalWrite(e, LOW);
  digitalWrite(f, LOW);
  digitalWrite(g, LOW);
  
}

void nine() //the 7-segment led display 9
{
  digitalWrite(a, LOW);
  digitalWrite(b, LOW);
  digitalWrite(c, LOW);
  digitalWrite(d, LOW);
  digitalWrite(e, HIGH);
  digitalWrite(f, LOW);
  digitalWrite(g, LOW);
  
}
/*******************************************/

void hitung_jam(long int x, int y)  // Counting Hour
{
  clearLEDs();//clear the 7-segment display screen
  if (y!= 0){
  pickDigit(0);//Light up 7-segment display d1
  pickNumber((x/36000));// get the value of thousand
  } else {
    if (!toggle) {
      digitalWrite(d1, HIGH);
      pickDigit(0);
      pickNumber((x/36000));
    } else digitalWrite(d1,LOW);
  }
 
  clearLEDs();//clear the 7-segment display screen
  if (y!= 0){  
  pickDigit(1);//Light up 7-segment display d2
  pickNumber((x/3600)%10);// get the value of hundred
  } else {
    if (!toggle) {
      pickDigit(1);
      pickNumber((x/3600)%10);
    } else digitalWrite(d2,LOW);
  }
  
  clearLEDs();//clear the 7-segment display screen
  if (y != 1) {
    pickDigit(2);//Light up 7-segment display d3
    pickNumber((x%3600)/600);//get the value of ten
    } else {
      if (!toggle) {
        pickDigit(2);//Light up 7-segment display d3
        pickNumber((x%3600)/600);//get the value of ten
      } else digitalWrite(d3, LOW);//Light d1 up 
    }
  
  clearLEDs();//clear the 7-segment display screen
  if (y != 1) {
    pickDigit(3);//Light up 7-segment display d4
    pickNumber(((x%3600)/60)%10);//Get the value of single digit
    } else {
      if (!toggle) {
        pickDigit(3);//Light up 7-segment display d4
        pickNumber(((x%3600)/60)%10);//Get the value of single digit
      } else digitalWrite(d4, LOW);//Light d1 up 
  }
}

void stopwatch(int x){
    clearLEDs();
    pickDigit(0); //Ngidupin Digit ke 1
    pickNumber((x%3600)/600);//nilai digitnya puluhan 
    
    clearLEDs();
    pickDigit(1); //Ngidupin Digit ke 2
    pickNumber(((x%3600)/60)%10);
   
    clearLEDs();
    pickDigit(2); //Ngidupin Digit ke 3
    pickNumber((x%60)/10);
    
    clearLEDs();
    pickDigit(3); //Ngidupin Digit ke 4
    pickNumber(x%10);
   
}
