#include "IRremote.h"
#include <LiquidCrystal.h>

#define PIN_RECEIVER 6   // Signal Pin for IR receiver
#define LED_BUZZER 5   // Signal pin for LED and Buzzer

IRrecv carIRRecv(PIN_RECEIVER);    // define receiver object
decode_results cmd;

LiquidCrystal lcd(7, 8, 9, 10, 11, 12);   // signal pin for LCD

bool sent = false;   // bool for indicating whether a signla was sent
bool endOfMessage = false;



/***************************************Setup()**********************************/
void setup()
{
  Serial.begin(9600);
  lcd.begin(16, 2);   // initialize LCD
  lcd.print("CAR SCREEN");     // this circuit would represent a car
  carIRRecv.enableIRIn(); // Start the receiver

/*****************************************
  // while a S.O.S is not sent
  while(!sent)
  {
    // Checks received an IR signal
    if (carIRRecv.decode(&cmd)) {
      // if we sent a S.O.S(0xFF629D) signal
      if(cmd.value == 0xFF629D)
      {
        // display S.O.S
        lcd.clear();
        lcd.print("CAR OUTPUT");
        Serial.print("!!!");
        lcd.setCursor(0, 1);
        translateIR();

        // turn on and off the LEDs and buzzer for five times
        for(int i = 0; i < 5; i++)
        {
          digitalWrite(LED_BUZZER, HIGH); // turn on
          delay(1000); // wait 1s
          digitalWrite(LED_BUZZER, LOW); // turn off
          delay(1000); // wait 1s
        } // end for
        sent = true; // message sent
      } // end if
      else{ // keep reading the next signal
        Serial.print("@");
        carIRRecv.resume();  // Receive the next value
      } // end else
    } // end if
  } // end while
************************************************/
} // end setup()

/***************************************loop()**********************************/
void loop()
{
  
  // Checks received an IR signal
  if (carIRRecv.decode(&cmd)) {
    translateIR();  // take action for the signal
    carIRRecv.resume();  // Receive the next value
  } // end if
} // end loop


/*************************************translateIR()**********************************/
void translateIR()
{
  // Takes command based on IR code received
  switch (cmd.value) {
    case 0xFF02FD: 
      lcd.setCursor(0, 1);
      lcd.print("We received");
      delay(2000);
      lcd.setCursor(0, 1);
      lcd.print("                ");
      lcd.setCursor(0, 1);
      lcd.print("your cooedinate.");
      break; //PAUSE
    case 0xFFA25D: 
      lcd.home();
      lcd.print("                ");
      lcd.home();
      lcd.write("MESSAGE END");
      break;//POWER
    case 0xFF629D: // VOL+
      lcd.setCursor(0, 1);
      lcd.print("Sending S.O.S");
      break;
    case 0xFFB04F:  // ST/REPT, RTC
      lcd.clear();
      lcd.home();
      lcd.print("RADIO OUTPUT");
      break;
    case 0xFF22DD:  // |<<
      lcd.setCursor(0, 1);
      lcd.print("We received");
      delay(2000);
      lcd.setCursor(0, 1);
      lcd.print("                ");
      lcd.setCursor(0, 1);
      lcd.print("your signal.");
      delay(2000);
      lcd.setCursor(0, 1);
      lcd.print("                ");
      lcd.setCursor(0, 1);
      lcd.print("Please enter ");
      delay(2000);
      lcd.setCursor(0, 1);
      lcd.print("                ");
      lcd.setCursor(0, 1);
      lcd.print("your coordinate.");
      break;
    case 0xFFC23D:  // >>|
      lcd.setCursor(0, 1);
      lcd.print("We will");
      delay(1000);
      lcd.setCursor(0, 1);
      lcd.print("                ");
      lcd.setCursor(0, 1);
      lcd.print("arrive shortly");
      break;
    case 0xFF906F: // UP, CTR, ignor
      lcd.print("*"); 
      endOfMessage = false;
      while(!endOfMessage)
      {
        // Checks received an IR signal
        if (carIRRecv.decode(&cmd)) {
          // if we sent a END(162) signal
          if(cmd.value == 0xFFA25D)
          {
            endOfMessage = true;
            lcd.print("*");
          } // end if
          else{ // keep reading the next signal
            carIRRecv.resume();  // Receive the next value
          } // end else
        } // end if
      } // end while
      break;
  } // end switch
} // end function
