# SimpleSNES

### Currently

A quick project that acts as a SNES to USB adapter

### Eventually

A project that can act as both a SNES to USB adapter and an input display program when connect between a real SNES
by letting the console control the latch and clock signals, passively monitoring the data line
and a controller.


## Dependencies

SimpleSNES depends on

- Arduino 1.6.6 or greater (tested on Arduino 1.6.11)

- [ArduinoJoystickLibrary](https://github.com/MHeironimus/ArduinoJoystickLibrary) library


## Features

- Buttons map to the same layout that a PS3 controller does

- Extremely simple hotplug support


## TODO

- Passthrough support

- Code cleanup

- DPAD / Left Stick toggle support

- SNES mouse support?

- SuperScope support (unlikely, I don't have a super scope OR CRT to test with :( )

- Confirm NES support