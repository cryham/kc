Keyboard Controler
==================

This my own USB Keyboard Controler code for Teensy 3.2 with TFT ST7735 color display.

Called K.C. ("Kacey") for short.

It features:
* Mapping menu, for remapping USB key codes to physical keys in real time, on running keyboard
* Layers (max 16)
* Sequences menu, for editing macros, passwords, etc.
* Visual keyboard drawn on display with current mappings. Layout is set in cpp file
* Mouse keys with move acceleration
* Anti-ghosting code, needed for foil keyboards without diodes
* Many color demos for display, e.g. Plasma, Wave, Polyhedrons 3D, etc.
* Loading and saving full keyboard setup on EEPROM (parameters, mappings, sequences)
* Testing page showing pressed keys info, matrix ghosting, etc.
* Menu for picking keys with custom group colors and filtering

It uses code from repositories:
* [PaulStoffregen cores](https://github.com/PaulStoffregen/cores/tree/master/teensy3) for teensy 3 core, reduced
* [sumotoy TFT_ST7735](https://github.com/sumotoy/TFT_ST7735/tree/1.0p1) for fast TFT ST7735 display
* [Adafruit-GFX-Library](https://github.com/adafruit/Adafruit-GFX-Library) for drawing, combined with above
* [Kiibohd](https://github.com/kiibohd/controller) small part, only keyboard matrix scan code

Some of my code (anti-ghosting, Gui, demos) are adopted and were already present in my [fork of kiibohd](https://github.com/cryham/controller),
done for my previous keyboards, info on [My Website](http://cryham.tuxfamily.org/archives/portfolio/crystal-keyboard-3-and-4).

Hardware
--------

The keyboard is made of:
* Teensy 3.2 (or 3.1), overclocked, stable at 144 MHz
* TFT LCD display ST7735, 160 x 128, 16bit color RGB565. Red PCB with SD card slot. Terribly low viewing angles.
* NPN transistor BC550. Connection from DAC A14 output through 4.7k resistor to base, collector to 3.3V, emitter through 47 ohm resistor to Display LED.

Licensing
---------

My sources are licensed GPLv3. For other sources used, see their repositories.
