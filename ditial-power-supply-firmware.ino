#include <LiquidCrystal.h>

#include <Keypad.h>


float incomingByte = 6;

float vol=6;

String s="";

char b=0;

volatile  char key;

volatile int count=0;

bool keyInput=false,c=0;

float keyInputNum=0;



// initialize the library with the numbers of the interface pins

LiquidCrystal lcd(25, 27, 29, 31, 33, 35);



const byte ROWS = 4; // Four rows

const byte COLS = 4; // Three columns

// Define the Keymap

char keys[ROWS][COLS] = {

{'1','2','3','V'},

{'4','5','6','B'},

{'7','8','9','C'},

{'.','0','#','D'}

};

// Connect keypad ROW0, ROW1, ROW2 and ROW3 to these Arduino pins.

byte rowPins[ROWS] = {53,51, 49, 47 };

// Connect keypad COL0, COL1 and COL2 to these Arduino pins.

byte colPins[COLS] = { 45, 43, 41,39 };



// Create the Keypad

Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );



void setup() {

Serial.begin(9600); // set up Serial library at 9600 bps

pinMode(23, OUTPUT);

digitalWrite(23, LOW);



//buzzer pin

pinMode(37, OUTPUT);

digitalWrite(37, HIGH);



pinMode(A0, INPUT);



// set up the LCD's number of columns and rows:

lcd.begin(16, 2);

// Print a message to the LCD.



//interrupt

pinMode(2, INPUT);

attachInterrupt(0, keypad, FALLING);



setVoltage();

}



void loop() {



if(!keyInput){

lcd.clear();



lcd.setCursor(0,0);

lcd.print("Digi Power Suply");



lcd.setCursor(5,1);

lcd.print(vol);

lcd.print(" V");



if (Serial.available() > 0) {

// read the incoming byte:

incomingByte = Serial.parseFloat();

if(incomingByte>=1.3 && incomingByte<=12){

vol=incomingByte;

Serial.print(vol);

//Serial.println(incomingByte, DEC);

setVoltage();

}//end inner if

}

int x = analogRead(A0);

float v=x*0.00488758*4.83;

if(v-vol>0){

if(incomingByte-1>=0)

incomingByte--;

else

incomingByte=255;

}

else if(v-vol<0){

if(incomingByte+1<=255)

incomingByte++;

else

incomingByte=0;

}

analogWrite(13, incomingByte);

//bounce back to serial, the voltage value

delay(500);

}//end if

//hardware keypad processing

else{

char cursorCount=7;

String s="";

key='t';

while(key!='V'){

key = kpd.getKey();

delay(50);

if(key)  // Check for a valid key.

{

digitalWrite(37, LOW);

delay(50);

s+=key;

lcd.setCursor(cursorCount++,0);

lcd.print(key);

digitalWrite(37, HIGH);

}

}//end while

keyInputNum=StrToFloat(s);

if(keyInputNum>=1.3 && keyInputNum<=12){

incomingByte=keyInputNum;

vol=incomingByte;

Serial.print(vol);

setVoltage();

}//end inner if

keyInput=false;

}//end else

}



void keypad(){

digitalWrite(37, LOW);

lcd.clear();

lcd.setCursor(0,0);

lcd.print("Input:");



keyInput=true;



delay(70);

digitalWrite(37, HIGH);

}



void setVoltage(){

vol=incomingByte;

                incomingByte-=1.18;

incomingByte/=2.8;

                incomingByte*=100;

incomingByte/=5;

incomingByte*=255;

incomingByte=incomingByte/100;

analogWrite(13, incomingByte);

delay(1000);

}



float StrToFloat(String str){

char carray[str.length() + 1]; //determine size of the array

str.toCharArray(carray, sizeof(carray)); //put str into an array

return atof(carray);

}
