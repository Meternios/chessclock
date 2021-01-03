// include the library code:
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 8, rw = 9, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, rw, d4, d5, d6, d7);

int minutesOfeachPlayer = 20; //To be changed

int time = minutesOfeachPlayer * 60;

int minutesP1 = time / 60;
int minutesP2 = time / 60;

int secondsP1 = 0;
int secondsP2 = 0;

int currentPlayer = 0; //Set to zero so the clock does not start until a button is pressed


int inputLeft = 0;
int inputRight = 0;
int pinLeft = 12;
int pinRight = 13;

void setup() {
  Serial.begin(38400);
  lcd.begin(16, 2);
  
  resetLcd();

  //Input Pins
  pinMode(pinLeft,INPUT);
  pinMode(pinRight,INPUT);

  //Output Pins
  pinMode(A0,OUTPUT);
  pinMode(A1,OUTPUT);
  pinMode(A2,OUTPUT);
  pinMode(A3,OUTPUT);
  pinMode(A4,OUTPUT);
  pinMode(A5,OUTPUT);
  

  //Timer 1 definieren
  noInterrupts();     //Alle Interupts temporär abschalten
  TCCR1A = 0;
  TCCR1B = 0;

  TCNT1 = 3036;            //Timer nach Formel vorladen
  TCCR1B |= (1 << CS12);    //256 als Prescale-Wert spezifizieren
  TIMSK1 |= (1 << TOIE1);   //Timer Overflow Intrrupt aktivieren
  interrupts();             //alle Interrupts aktivieren
}

ISR(TIMER1_OVF_vect)      //Call function setTimes with ISR
{
  TCNT1 = 3036;          //Zaehler erneut vorbelegen
  setTimes();
}

void loop() {
  
  inputLeft = digitalRead(pinLeft);
  inputRight = digitalRead(pinRight);

  if(inputLeft == HIGH){
    currentPlayer = 2;
  }

  if(inputRight == HIGH){
    currentPlayer = 1;
  }

  if(inputRight == HIGH && inputLeft == HIGH){
    
    //Reset
    resetTime();
    resetLcd();
    delay(1000);

  }
}

bool setTimes(){
  if(currentPlayer == 1){
    //If Minute is over
    if(secondsP1 == -1){
      secondsP1 = 59;
      minutesP1--;
      if(minutesP1 == -1){
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Green lost :(");
        lcd.setCursor(0, 1);
        lcd.print("Red won :)");
        noInterrupts();
        resetTime();
        return false;
      }
    }
    if(secondsP1 == 9){ //If seconds below 10
      lcd.setCursor(3, 1);
      lcd.print("0");
    }
    if(minutesP1 == 9){ //If minutes below 10
      lcd.setCursor(0, 1);
      lcd.print("0");
    }
  
    if(minutesP1 < 10){
      lcd.setCursor(1, 1);
    }else {
      lcd.setCursor(0, 1);
    }
    lcd.print(minutesP1);

    lcd.setCursor(2, 1);
    lcd.print(":");
  
    if(secondsP1 < 10){
      lcd.setCursor(4, 1);
    }else {
      lcd.setCursor(3, 1);
    }
    lcd.print(secondsP1);
    
    secondsP1--;
  }else if(currentPlayer == 2){
    //If Minute is over
    if(secondsP2 == -1){
      secondsP2 = 59;
      minutesP2--;
      if(minutesP2 == -1){
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Red lost :(");
        lcd.setCursor(0, 1);
        lcd.print("Green won :)");
        noInterrupts();
        resetTime();
        return false;
      }
    }
    if(secondsP2 == 9){ //If seconds below 10
      lcd.setCursor(14, 1);
      lcd.print("0");
    }
    if(minutesP2 == 9){ //If minutes below 10
      lcd.setCursor(11, 1);
      lcd.print("0");
    }
  
    if(minutesP2 < 10){
      lcd.setCursor(12, 1);
    }else {
      lcd.setCursor(11, 1);
    }
    lcd.print(minutesP2);

    lcd.setCursor(13, 1);
    lcd.print(":");
  
    if(secondsP2 < 10){
      lcd.setCursor(15, 1);
    }else {
      lcd.setCursor(14, 1);
    }
    lcd.print(secondsP2);
    
    secondsP2--;
  }

  return true;
}

void resetTime(){
  time = minutesOfeachPlayer * 60;

  minutesP1 = time / 60;
  minutesP2 = time / 60;
  
  secondsP1 = 0;
  secondsP2 = 0;
  
  currentPlayer = 0; //Set to zero so the clock does not start until a button is pressed

}

void resetLcd(){
  lcd.clear();
  //Title
  lcd.setCursor(0, 0);
  lcd.print("Schach Uhr");
  
  //Player 1
  if(minutesP1 < 10){
    lcd.setCursor(1, 1);
  }else {
    lcd.setCursor(0, 1);
  }
  lcd.print(minutesP1);
  lcd.setCursor(2, 1);
  lcd.print(":");
  if(secondsP1 < 10){
    lcd.setCursor(4, 1);
  }else {
    lcd.setCursor(3, 1);
  }
  lcd.print(secondsP1);

  //Player 2
  if(minutesP2 < 10){
    lcd.setCursor(12, 1);
  }else {
    lcd.setCursor(11, 1);
  }
  lcd.print(minutesP2);
  lcd.print(":");
  if(secondsP2 < 10){
    lcd.setCursor(15, 1);
  }else {
    lcd.setCursor(14, 1);
  }
  lcd.print(secondsP2);

  //Set 0 if below 10
  if(minutesP1 < 10){
    lcd.setCursor(0, 1);
    lcd.print("0");
  }
  if(secondsP1 < 10){
    lcd.setCursor(3, 1);
    lcd.print("0");
  }
  if(minutesP2 < 10){
    lcd.setCursor(11, 1);
    lcd.print("0");
  }
  if(secondsP2 < 10){
    lcd.setCursor(14, 1);
    lcd.print("0");
  }
}
