/*********************************LIBRARIES*************************************/
#include "IRremote.h"
#include <LiquidCrystal.h>

/******************************PIN DECLERATION *********************************/
#define PIN_RECEIVER 2   // Signal Pin for IR receiver
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);   // signal pin for LCD


/****************************GLOBAL VARIABLES***********************************/
// bool indicator for the end of an RTC(radio to car) or CTR(car to radio) message
bool endOfMessage = false; 
// array for car location, coor[0] = x coordinate, coor[1] = y coordinate
float coor [2]; 
int index = 0;    // coor[] index
float distance = 0;   // distance between the car and the radio
float angle = 0;    // angle between the car and the radio


/************************OBJECT DECLERATION*************************************/
IRrecv radIRRecv(PIN_RECEIVER);    // define receiver object
decode_results cmd;   // cmd stores the decoded message received by the IR receiver


/***************************************Setup()**********************************/
// initialize serial monitor, LCD, and IR receiver
void setup()
{
  lcd.begin(16, 2);   // initialize LCD
  lcd.print("RADIO SCREEN");     // this circuit would represent a radio station
  radIRRecv.enableIRIn(); // start the receiver
} // end setup()

/***************************************loop()**********************************/
void loop()
{
  // use if to check any received signal
  if (radIRRecv.decode(&cmd)) {
    Serial.print("Value: ");
    Serial.println(cmd.value, HEX);
    //translateIR();  // take action for the signal
    radIRRecv.resume();  // Receive the next value
  } // end if
} // end loop


/*************************************translateIR()**********************************/
void translateIR()
{
  // Takes command based on IR code received
  switch (cmd.value) {
    case 0xFFA857:
      calculateDistance();
      calculateDegree();
      lcd.clear();
      lcd.home();
      lcd.print("Calculating");
      lcd.setCursor(0, 1);
      lcd.print("distance...");
      delay(2000);
      lcd.clear();
      lcd.home();
      lcd.print("The car is");
      lcd.setCursor(0, 1);
      lcd.print(distance);
      lcd.print(" miles away,");
      delay(2000);
      lcd.clear();
      lcd.home();
      lcd.print("Sending");
      lcd.setCursor(0, 1);
      lcd.print("support...");
      break; //VOL-
    case 0xFF02FD: 
      lcd.clear();
      lcd.home();
      lcd.print(coor[0]);
      lcd.print(" ");
      lcd.print(coor[1]);
      break; //PAUSE
    case 0xFFE01F:
      lcd.print(",");
      break; //DOWN
    case 0xFF6897: 
      lcd.print("0");
      coor[index] = 0;
      index++;
      break;
    case 0xFF30CF: 
      lcd.print("1");
      coor[index] = 1;
      index++;
      break;
    case 0xFF18E7: 
      lcd.print("2");
      coor[index] = 2;
      index++;
      break;
    case 0xFF7A85: 
      lcd.print("3");
      coor[index] = 3;
      index++;
      break;
    case 0xFF10EF: 
      lcd.print("4");
      coor[index] = 4;
      index++;
      break;
    case 0xFF38C7: 
      lcd.print("5");
      coor[index] = 5;
      index++;
      break;
    case 0xFF5AA5: 
      lcd.print("6");
      coor[index] = 6;
      index++;
      break;
    case 0xFF42BD: 
      lcd.print("7");
      coor[index] = 7;
      index++;
      break;
    case 0xFF4AB5: 
      lcd.print("8");
      coor[index] = 8;
      index++;
      break;
    case 0xFF52AD: 
      lcd.print("9");
      coor[index] = 9;
      index++;
      break;
    case 0xFFA25D: // POWER
      //lcd.clear();
      lcd.home();
      lcd.write("                ");
      lcd.home();
      lcd.write("MESSAGE END");
      break;
    case 0xFF629D: // VOL+
      lcd.setCursor(0, 1);
      lcd.print("S.O.S");
      break;
    case 0xFF906F:  // UP, CTR
      lcd.clear();
      lcd.home();
      lcd.print("CAR OUTPUT");
      lcd.setCursor(0, 1);
      break;
    case 0xFFB04F: // ST/REPT, RTC
      lcd.print("*"); 
      endOfMessage = false;
      while(!endOfMessage)
      {
        // Checks received an IR signal
        if (radIRRecv.decode(&cmd)) {
          // if we sent a END(162) signal
          if(cmd.value == 0xFFA25D)
          {
            endOfMessage = true;
            lcd.print("*");
          } // end if
          else{ // keep reading the next signal
            radIRRecv.resume();  // Receive the next value
          } // end else
        } // end if
      } // end while
      break;
  } // end switch
} // end TranslateIR()

void calculateDistance()
{
  int xSquare = coor[0] * coor[0];
  int ySquare = coor[1] * coor[1];
  int sum = xSquare + ySquare;
  distance = sqrt(sum);
}

void calculateDegree()
{
  int xCoor = coor[0];
  int yCoor = coor[1];
  angle = atan(xCoor/yCoor);
  Serial.print(angle);
  angle = angle * 180;
  angle = angle / 3.1415926;
  Serial.print(angle);
}
