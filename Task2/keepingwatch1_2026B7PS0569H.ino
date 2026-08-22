// C++ code
//

// AARUSH VASHISTH
// 2026B7PS0569H

#include <LiquidCrystal.h>

// defining the pins
int pushPin = 7;
int lightPin = A0;
int LEDpin = 5;
int trigPin = 4;
int echoPin = 3;
int buzzPin = 2;

// Push button states
int buttonState = 0, lastButtonState = 0, anchorDropped = 0;
// State of the sea/weather & special WRECKED case
enum seaState
{
  OPEN_SEA,
  STORM,
  CHARYBDIS,
  WRECKED
};
seaState currentState, lastState;

float const soundSpeed = 0.0348; // speed of sound (constant) (cm/micro sec)
unsigned long LEDtime = 0, stopwatch = 0; // Time variables

//                RS, EN, D4, D5, D6, D7
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);

// function prototypes
void printer(int currentState);
float calc_distance();

void setup()
{
  lcd.begin(16,2);
  
  pinMode(pushPin, INPUT);
  pinMode(lightPin, INPUT);
  pinMode(LEDpin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzPin, OUTPUT);
  
  lcd.print("ARTEMIS' INTERN");
  delay(2000);
  lcd.clear();
  
  currentState = OPEN_SEA;
  lastState = OPEN_SEA;
  lcd.print("OPEN SEA");
  
  Serial.begin(9600);
}

void loop()
{
  buttonState = digitalRead(pushPin);
  if(buttonState == HIGH && lastButtonState == LOW)
  {
    // Flips the anchor switch only when the button is pressed (high)
    // and the previous state was low
    if(anchorDropped == 0 && currentState != WRECKED)
    {
      anchorDropped = 1;
      lcd.clear();
      lcd.print("ANCHOR");
      lcd.setCursor(0,1);
      lcd.print("DROPPED");
    }
    else
    {
      anchorDropped = 0;
      printer(currentState);
      stopwatch = millis();
    }
  }
  lastButtonState = buttonState;
  
  if(anchorDropped != 1 && currentState != WRECKED)
  {
    // Check for STORM only if current state is OPEN SEA or STORM
    if(currentState == OPEN_SEA || currentState == STORM)
    {
      // Max value of the photoresistor was found to be 679
      int val = analogRead(lightPin);
  	  int max = 679;
  	  int threshold = (int)(max/2);
      // Check for STORM threshold
  	  if (val < threshold)
  	  {
        currentState = STORM;
        if(lastState != currentState)
        {
          printer(currentState);
          lastState = currentState;
        }
      }
      else
      {
        currentState = OPEN_SEA;
        stopwatch = millis();
        // Important if block which updates LCD only when the currentState
        // is different from the previous state
        if(lastState != currentState)
        {
          printer(currentState);
          lastState = currentState;
        }
      }
      if(currentState == STORM)
      {
        // LED blink code, without using delay()
        if(millis() - LEDtime > 1000)
        {
          digitalWrite(LEDpin, HIGH);
          LEDtime = millis();
        }
        else
          digitalWrite(LEDpin, LOW);
      }
    } 
    
    // Check for CHARYBDIS only if current state is OPEN SEA or CHARYBDIS
    if(currentState == OPEN_SEA || currentState == CHARYBDIS)
    {
      float distance = calc_distance(); // returns distance
      if(distance <= 100) // Threshold for CHARYBDIS
      {
        currentState = CHARYBDIS;
        if(lastState != currentState)
        {
          printer(currentState);
          lastState = currentState;
        }
      }
      else
      {
        currentState = OPEN_SEA;
        stopwatch = millis();
        // Important if block which updates LCD only when the currentState
        // is different from the previous state
        if(lastState != currentState)
        {
          printer(currentState);
          lastState = currentState;
        }
      }
      // Buzzer if-else block
      if(currentState == CHARYBDIS)
      {
        digitalWrite(buzzPin, HIGH);
      }
      else
      {
        digitalWrite(buzzPin, LOW);
      }
    }
    // Check for WRECKED
    if(currentState == STORM || currentState == CHARYBDIS)
    {
      if(millis() - stopwatch >= 5000)
      {
        currentState = WRECKED;
        if(lastState != currentState)
        {
          printer(currentState);
          lastState = currentState;
        }
      }
    }
  }
  else if(currentState == WRECKED)
  {
    if(lastState != currentState)
    {
      printer(currentState);
      lastState = currentState;
    }
    digitalWrite(LEDpin, LOW);
    digitalWrite(buzzPin, LOW);
  }
  else if(currentState != WRECKED && anchorDropped == 1)
  {
    // In case of anchor drop, no event matters.
    // Ship stays safe and hence all alarms/events are off
    digitalWrite(LEDpin, LOW);
    digitalWrite(buzzPin, LOW);
  }
}

void printer(int currentState)
{
  // Prints the currentState when called
  lcd.clear();
  switch(currentState)
  {
    case OPEN_SEA:
    lcd.print("OPEN SEA");
    break;
    
    case STORM:
    lcd.print("STORM");
    break;
    
    case CHARYBDIS:
    lcd.print("CHARYBDIS");
    break;
    
    case WRECKED:
    lcd.print("WRECKED");
    break;
  }
}

float calc_distance()
{
  /* Sends a signal and pingTime is calculated
  as the time for which echoPin stays HIGH */
  digitalWrite(trigPin, LOW);
  delayMicroseconds(10);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(10);
  int pingTime = pulseIn(echoPin, HIGH);
  delayMicroseconds(25);
  // Calculate distance by using dist =  speed*time / 2
  // since ping travels the same distance twice (to and fro)
  float distance = (soundSpeed*pingTime)/2;
  return distance;
}
    