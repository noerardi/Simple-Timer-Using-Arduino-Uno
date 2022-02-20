const int cathodePins[4] = {10,11,12,13}; //D4 D3 D2 D1
const int ssdPins[7] = {3,4,5,6,7,8,9};
const int dotPin = 2;
static int gCounter = 0 ;
static int buttonPin = 2;
bool state = true;

byte numbers[10] = {
  B00111111, // 0   "0"          AAA
  B00000110, // 1   "1"         F   B
  B01011011, // 2   "2"         F   B
  B01001111, // 3   "3"          GGG
  B01100110, // 4   "4"         E   C
  B01101101, // 5   "5"         E   C
  B01111101, // 6   "6"          DDD
  B00000111, // 7   "7"
  B01111111, // 8   "8"
  B01101111, // 9   "9"  
  };

void setup() {
  // put your setup code here, to run once:
   for (int i = 0 ; i < 4 ; ++i)
    pinMode(cathodePins[i],OUTPUT);

  for (int i = 0 ; i < 7 ; ++i)
    pinMode(ssdPins[i],OUTPUT);

   pinMode(dotPin,OUTPUT);

  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = 15624;// = (16*10^6) / (1*1024) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS10 and CS12 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);  
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  pinMode(buttonPin,INPUT);
  attachInterrupt(digitalPinToInterrupt(buttonPin),buttonTapped, RISING);

}

void findDigits(int digits[4],int counter)
{
  int digitCounter = 0 ;
  while (counter > 0)
  {
    digits[digitCounter] = counter % 10 ;
    counter = counter / 10 ;
    digitCounter++;     
  }  
}

void displayNumber(int counter)
{
  int digits[4] = {0};

  findDigits(digits,counter);

  //D4
  digitalWrite(cathodePins[0],LOW);
  digitalWrite(cathodePins[1],HIGH);
  digitalWrite(cathodePins[2],HIGH);
  digitalWrite(cathodePins[3],HIGH);
  
  for ( int j = 0 ; j < 7 ; ++j )
  {
     if ( numbers[digits[3]] & 1 << j )  
       digitalWrite(ssdPins[j],HIGH);
     else
       digitalWrite(ssdPins[j],LOW);
  }   
  //
  delay(5);
  //D3
  digitalWrite(cathodePins[0],HIGH);
  digitalWrite(cathodePins[1],LOW);
  digitalWrite(cathodePins[2],HIGH);
  digitalWrite(cathodePins[3],HIGH);
  
  for ( int j = 0 ; j < 7 ; ++j )
  {
     if ( numbers[digits[2]] & 1 << j )  
       digitalWrite(ssdPins[j],HIGH);
     else
       digitalWrite(ssdPins[j],LOW);
  }   
  //
  delay(5);
  //D2
  digitalWrite(cathodePins[0],HIGH);
  digitalWrite(cathodePins[1],HIGH);
  digitalWrite(cathodePins[2],LOW);
  digitalWrite(cathodePins[3],HIGH);
  
  for ( int j = 0 ; j < 7 ; ++j )
  {
     if ( numbers[digits[1]] & 1 << j )  
       digitalWrite(ssdPins[j],HIGH);
     else
       digitalWrite(ssdPins[j],LOW);
  }   
  //
  delay(5);
  //D1
  digitalWrite(cathodePins[0],HIGH);
  digitalWrite(cathodePins[1],HIGH);
  digitalWrite(cathodePins[2],HIGH);
  digitalWrite(cathodePins[3],LOW);
  
  for ( int j = 0 ; j < 7 ; ++j )
  {
     if ( numbers[digits[0]] & 1 << j )  
       digitalWrite(ssdPins[j],HIGH);
     else
       digitalWrite(ssdPins[j],LOW);
  }   
  //
  delay(5);
}

void loop() {
  // put your main code here, to run repeatedly:
 displayNumber(gCounter);
}

ISR(TIMER1_COMPA_vect){  
  if (state){
    gCounter++;
    if (gCounter == 10000)
    gCounter = 0;
  }
}

void buttonTapped(){
 state = !state;
  delay(10);
}