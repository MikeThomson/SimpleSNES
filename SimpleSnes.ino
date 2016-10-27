// relies on https://github.com/MHeironimus/ArduinoJoystickLibrary
#include <DynamicHID.h>
#include <Joystick.h>

// Pin definitions
#define SNES_LATCH 8
#define SNES_DATA 7
#define SNES_CLOCK 9

// SNES bitfield constants
#define SNES_B 0
#define SNES_Y 1
#define SNES_SELECT 2
#define SNES_START 3

#define SNES_UP 4
#define SNES_DOWN 5
#define SNES_LEFT 6
#define SNES_RIGHT 7

#define SNES_A 8
#define SNES_X 9
#define SNES_L 10
#define SNES_R 11

// PS3 compatible HID gamepad button IDs
#define PS3_CROSS 2
#define PS3_SQUARE 1
#define PS3_CIRCLE 3
#define PS3_TRIANGLE 4

#define PS3_START 10
#define PS3_SELECT 9
#define PS3_HOME 13

#define PS3_R1 6
#define PS3_L1 5


Joystick_ Joystick(
  JOYSTICK_DEFAULT_REPORT_ID,
  JOYSTICK_TYPE_JOYSTICK,
  13, // button count,
  1, // hat count
  true, // X axis
  true, // Y axis
  true, // Z axis
  false, // Rx axis
  false, // Ry axis
  true, // Rz axis
  false, // Rudder axis
  false, // Throttle axis
  false, // Accelerator axis
  false, // Brake axis
  false // Steering axis
  
);

bool buttonStates[16];

void setup() {
  // put your setup code here, to run once:
  pinMode(SNES_LATCH, OUTPUT);
  pinMode(SNES_CLOCK, OUTPUT);
  pinMode(SNES_DATA, INPUT);

  initJoy();
  Joystick.begin(false);
}

void loop() {
  updateButtonStates();
  sendButtons();

  delay(7); // poll at ~120hz for now
}

void updateButtonStates() {
  latch();

  // clock in button values
  for(int i=0;i<16;i++) {
    buttonStates[i] = !clockIn();
  }
  digitalWrite(SNES_CLOCK, HIGH); // leave high during back porch

  // weird hack to make sure a controller is connected
  // bits 13-16 should be unpressed. if they read active, just assume nothing is connected / pressed
  // definitely should figure out if this is used by any controllers (mouse?) that would make this
  // assumption invalid
  if(buttonStates[15] == true) {
    for(int i=0;i<16;i++) {
      buttonStates[i] = false;
    }
  }
}

void latch() {
  digitalWrite(SNES_LATCH, HIGH);
  delayMicroseconds(12);
  digitalWrite(SNES_LATCH, LOW);
  delayMicroseconds(6);
}

bool clockIn() {
  bool reading;
  
  digitalWrite(SNES_CLOCK, HIGH);
  delayMicroseconds(6);
  
  digitalWrite(SNES_CLOCK, LOW);
  reading = digitalRead(SNES_DATA);
  delayMicroseconds(6);
  return reading;
}

void sendButtons() {
  Joystick.setButton(PS3_CROSS - 1, buttonStates[SNES_B]);
  Joystick.setButton(PS3_SQUARE - 1, buttonStates[SNES_Y]);
  Joystick.setButton(PS3_CIRCLE - 1, buttonStates[SNES_A]);
  Joystick.setButton(PS3_TRIANGLE - 1, buttonStates[SNES_X]);

  Joystick.setButton(PS3_R1 - 1, buttonStates[SNES_R]);
  Joystick.setButton(PS3_L1 - 1, buttonStates[SNES_L]);
  
  Joystick.setButton(PS3_START - 1, buttonStates[SNES_START]);
  Joystick.setButton(PS3_SELECT - 1, buttonStates[SNES_SELECT]);

  Joystick.setHatSwitch(0, getHat());

  Joystick.sendState();
}

int getHat() {
  bool up = buttonStates[SNES_UP];
  bool down = buttonStates[SNES_DOWN];
  bool left = buttonStates[SNES_LEFT];
  bool right = buttonStates[SNES_RIGHT];

  if(up) {
    if(right) 
      return 45;
    if(left)
      return 315;
    return 0;
  }

  if(right) {
    if(down)
      return 135;
    return 90;
  }

  if(down) {
    if(left)
      return 225;
    return 180;
  }

  if(left) {
    return 270;
  }

  return -1;
}

void initJoy() {
  Joystick.setXAxis(0x200);
  Joystick.setYAxis(0x200);
  Joystick.setZAxis(0x200);
  Joystick.setRzAxis(0x200);
}

