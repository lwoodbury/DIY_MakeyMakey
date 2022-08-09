/*  Code inspired by this Instructable:
 *   https://www.instructables.com/DIY-Makey-Makey-With-Arduino-Leonardo/
 *   
 *   Adapted for 12 analogue input pins available on Arduino micro
 *  
 * 
 */

#include <Keyboard.h>
#include <movingAvg.h>


// Original values were 200 and then 600
const int PressedMaxThreshold = 200;
const int ReleasedMinThreshold = 300;
const byte PinCount = 12;

// Define pins to use and output key presses to map
const byte InputPins[PinCount] = {A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11};
const char KeyCodes[PinCount] = {KEY_UP_ARROW, KEY_LEFT_ARROW, KEY_DOWN_ARROW, KEY_RIGHT_ARROW, 'w', 'a', 's', 'd', ' ', 'q', 'e', 'x'};

// Define a struct to hold our data
struct TouchInput
{
  byte analogPin;
  char keycode;
  movingAvg filter = movingAvg(20);
  boolean wasPressed = false;
};


TouchInput Pins[PinCount];


void setup()
{
  //Serial.begin(115200);

//Set up the pins and map to key presses
  for (int i = 0; i < PinCount; i++)
  {
    Pins[i].analogPin = InputPins[i];
    Pins[i].keycode = KeyCodes[i];
    Pins[i].filter.begin();
  }
}




void loop()
{
  // Read the analog pins and save previous and current readings between loops
  for (int i = 0; i < PinCount; i++)
  {
    float currentAverage = Pins[i].filter.reading(analogRead(Pins[i].analogPin));
    boolean previousState = Pins[i].wasPressed;
    boolean currentState = previousState; // Default if in the dead zone

    // Logic...
    if (currentAverage < PressedMaxThreshold)
    {

      currentState = true;      // Pressed

    }
    else if  (currentAverage > ReleasedMinThreshold)
    {

      currentState = false;      // Released
    }


    if (currentState != previousState)
    {
      if (currentState)
        Keyboard.press(Pins[i].keycode);
      else
        Keyboard.release(Pins[i].keycode);
    }
    Pins[i].wasPressed = currentState;
  }
}
