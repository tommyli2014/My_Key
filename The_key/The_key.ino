

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
  #include "Platform.h"
  #include "SoftwareSerial.h"
#ifndef CDC_ENABLED
  // Shield Jumper on SW
  SoftwareSerial port(12,13);
#else
  // Shield Jumper on HW (for Leonardo)
  #define port Serial1
#endif
#else // Arduino 0022 - use modified NewSoftSerial
  #include "WProgram.h"
  #include "NewSoftSerial.h"
  NewSoftSerial port(12,13);
#endif

#include "EasyVR.h"

EasyVR easyvr(port);

#define SND_BEEP        1

//Groups and Commands
enum Groups
{
  GROUP_0  = 0,
 GROUP_3  = 3,
};

enum Group0 
{
  G0_HELLO = 0,
};

enum Group3 //I used group 3 becasue group 1 and 2 are for my voice control robot commands from last year
{
  G3_STEAM = 0,
  G3_SKYPE = 1,
  G3_TWITCH = 2,
  G3_FACEBOOK = 3,
  G3_PREZI = 4,
  G3_COMCAST = 5,
};


EasyVRBridge bridge;

int8_t group, idx;

void setup()
{
  pinMode(8,OUTPUT);//For an LED to go
  digitalWrite(8,LOW);//initialzing LED to be off
  Keyboard.begin();
  
#ifndef CDC_ENABLED
  // bridge mode?
  if (bridge.check())
  {
    cli();
    bridge.loop(0, 1, 12, 13);
  }
  // run normally
  Serial.begin(9600);
  Serial.println("Bridge not started!");
#else
  // bridge mode?
  if (bridge.check())
  {
    port.begin(9600);
    bridge.loop(port);
  }
  Serial.println("Bridge connection aborted!");
#endif
  port.begin(9600);

  while (!easyvr.detect())
  {
    Serial.println("EasyVR not detected!");
    delay(1000);
  }

  easyvr.setPinOutput(EasyVR::IO1, LOW);
  Serial.println("EasyVR detected!");
  easyvr.setTimeout(5);
  easyvr.setLanguage(0);

  group = EasyVR::TRIGGER; //<-- start group (customize)
}

void action();

void loop()
{
  easyvr.setPinOutput(EasyVR::IO1, HIGH); // LED on (listening)
  pinMode(EasyVR::IO1, INPUT);
  pinMode(8,OUTPUT);
  
  Serial.print("Say a command in Group ");
  easyvr.playSound(SND_BEEP,EasyVR::VOL_FULL);
  Serial.println(group);
  easyvr.recognizeCommand(group);

  do
  {
    // can do some processing while waiting for a spoken command
  }
  while (!easyvr.hasFinished());
  
  easyvr.setPinOutput(EasyVR::IO1, LOW); // LED off

  idx = easyvr.getWord();
  if (idx >= 0)
  {
    // built-in trigger (ROBOT)
    // group = GROUP_X; <-- jump to another group X
    return;
  }
  idx = easyvr.getCommand();
  if (idx >= 0)
  {
    // print debug message
    uint8_t train = 0;
    char name[32];
    Serial.print("Command: ");
    Serial.print(idx);
    if (easyvr.dumpCommand(group, idx, name, train))
    {
      Serial.print(" = ");
      Serial.println(name);
    }
    else
      Serial.println();
    easyvr.playSound(0, EasyVR::VOL_FULL);
    // perform some action
    action();
  }
  else // errors or timeout
  {
    if (easyvr.isTimeout())
      Serial.println("Timed out, try again...");
    int16_t err = easyvr.getError();
    if (err >= 0)
    {
      Serial.print("Error ");
      Serial.println(err, HEX);
    }
  }
}

void action()
{
    switch (group)
    {
    case GROUP_0:
      switch (idx)
      {
      case G0_HELLO:
        Serial.println("Command recieved");
        easyvr.playSound(SND_BEEP,EasyVR::VOL_FULL); 
         group = GROUP_3;
        // write your action code here
        // group = GROUP_X; <-- or jump to another group X for composite commands
        break;
      }
      break;
    case GROUP_3:
      switch (idx)
      {
     // case G1_ON:
     case G3_STEAM:
       Keyboard.print("type your username here");
       Keyboard.press(KEY_TAB);
       delay(100);
       Keyboard.print("type your password here");
       Keyboard.press(KEY_RETURN);
    delay(100);
    Keyboard.releaseAll();
       //pinMode(8,OUTPUT);
        //digitalWrite(8,HIGH);
       // Serial.println("LED On n G1On");
        // write your action code here
        // group = GROUP_X; <-- or jump to another group X for composite commands
        break;
        
      case G3_SKYPE:
         Keyboard.print("type your username here");
         Keyboard.press(KEY_TAB);
         delay(100);
         Keyboard.print("type your password here");
         Keyboard.press(KEY_RETURN);
         delay(100);
         Keyboard.releaseAll();
         break;
         
        case G3_TWITCH:
         Keyboard.print("type your username here");
         Keyboard.press(KEY_TAB);
         delay(100);
         Keyboard.print("type your password here");
         Keyboard.press(KEY_RETURN);
         delay(100);
         Keyboard.releaseAll();
         break; 
         
        case G3_FACEBOOK:
         Keyboard.print("type your username here");
         Keyboard.press(KEY_TAB);
         delay(100);
         Keyboard.print("type your password here");
         Keyboard.press(KEY_RETURN);
         delay(100);
         Keyboard.releaseAll();
         break; 
         
        case G3_PREZI:
         Keyboard.print("type your username here");
         Keyboard.press(KEY_TAB);
         delay(100);
         Keyboard.print("type your password here");
         Keyboard.press(KEY_RETURN);
         delay(100);
         Keyboard.releaseAll();
         break;
        

     /* case G1_OFF:
       pinMode(8,OUTPUT);
        digitalWrite(8,LOW);
        Serial.println("LED OFF n G1On");
        // write your action code here
        // group = GROUP_X; <-- or jump to another group X for composite commands
        break;
      case G1_FORWARD:
      servo1.write(12);
     servo2.write(92);
        // write your action code here
        // group = GROUP_X; <-- or jump to another group X for composite commands
        break;
      case G1_BACKWARD:
      servo1.write(-25);
      servo2.write(108);
        // write your action code here
        // group = GROUP_X; <-- or jump to another group X for composite commands
        break;
      case G1_LEFT:
      servo1.write(+1);
      servo2.write(100);
        // write your action code here
        // group = GROUP_X; <-- or jump to another group X for composite commands
        break;
      case G1_RIGHT:
       servo1.write(6);
      servo2.write(106);
        // write your action code here
        // group = GROUP_X; <-- or jump to another group X for composite commands
        break;
      case G1_JAPANESE:
      Serial.println("Command recieved");
        easyvr.playSound(SND_BEEP,EasyVR::VOL_FULL); 
         group = GROUP_2;
        // write your action code here
        // group = GROUP_X; <-- or jump to another group X for composite commands
        break;
      case G1_STOP:
       servo1.write(2.99999);
      servo2.write(103.6);
        // write your action code here
        // group = GROUP_X; <-- or jump to another group X for composite commands
        break; */
      }
      break;
    /* case GROUP_2:
      switch (idx)
      {
      case G2_EIGO:
        Serial.println("Command recieved");
        easyvr.playSound(SND_BEEP,EasyVR::VOL_FULL); 
         group = GROUP_1;// write your action code here
        // group = GROUP_X; <-- or jump to another group X for composite commands
        break;
      case G2_HAKEN_SURU:
         servo1.write(105);
     servo2.write(-25);
        // write your action code here
        // group = GROUP_X; <-- or jump to another group X for composite commands
        break;
      case G2_USHIRO:
      servo1.write(92);
      servo2.write(12);
        // write your action code here
        // group = GROUP_X; <-- or jump to another group X for composite commands
        break;
      case G2_HIDARI:
        servo1.write(100);
      servo2.write(-1);
        // write your action code here
        // group = GROUP_X; <-- or jump to another group X for composite commands
        break;
      case G2_MIGI:
        servo1.write(106);
      servo2.write(4);
        // write your action code here
        // group = GROUP_X; <-- or jump to another group X for composite commands
        break;
      case G2_TSUKERU:
        pinMode(8,OUTPUT);
        digitalWrite(8,HIGH);
        Serial.println("LED On n G1On");
        // write your action code here
        // group = GROUP_X; <-- or jump to another group X for composite commands
        break;
      case G2_KESU:
       pinMode(8,OUTPUT);
        digitalWrite(8,LOW);
        Serial.println("LED OFF n G1On");
        // write your action code here
        // group = GROUP_X; <-- or jump to another group X for composite commands
        break;
      case G2_TOMERU:
        servo1.write(103);
      servo2.write(3);
        // write your action code here
        // group = GROUP_X; <-- or jump to another group X for composite commands
        break; */
      }
     // break;
  //  }
}
