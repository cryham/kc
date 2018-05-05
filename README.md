Keyboard Controler
==================

This my own USB Keyboard Controler code for Teensy 3.2 with TFT ST7735 color display.

Called K.C. ("Kacey") for short.

It allows editing everything on keyboard display, in real time. No other tools needed.

Features:
* Mapping - menu, for remapping USB key codes to physical keys.
* Layers
* Sequences - editor for macros, passwords, etc.
* In sequence commands - e.g. wait, change key delay, comment part, hide rest, and all mouse commands.
* Visual keyboard - drawn on display with current mappings. Physical layout is done in cpp file.
* Mouse keys - with move acceleration, also for wheel.

Also:
* Setup on EEPROM - Loading and saving full keyboard setup (parameters, mappings, sequences).
* Anti-ghosting code - needed for foil keyboards without diodes.
* Testing pages - showing pressed keys info, matrix ghosting, layer use, etc.
* Keys list - for picking, with custom group colors and filtering.
* Parameters - for Scan Setup, Mouse and Display, adjustable in menu.

Lastly:
* Many demos - with presets, e.g. Plasma, Wave, Fire, Polyhedrons 3D, etc.
* Falling blocks game - with random generated blocks, many presets and custom options
* Help pages with all GUI keys and colorful GUI Menu with 2 levels and 3 fonts

It uses code from repositories:
* [PaulStoffregen cores](https://github.com/PaulStoffregen/cores/tree/master/teensy3) for teensy 3 core, reduced
* [sumotoy TFT_ST7735](https://github.com/sumotoy/TFT_ST7735/tree/1.0p1) for fast TFT ST7735 display
* [Adafruit-GFX-Library](https://github.com/adafruit/Adafruit-GFX-Library) for drawing, combined with above
* [Kiibohd](https://github.com/kiibohd/controller) small part, only keyboard matrix scan code

Since I am using a buffer (40kB for 160x128), all drawing code fills it. On each frame, buffer gets sent to display, thus no flickering.
All unnecessary code for drawing from libraries was removed.

Displaying GUI and scanning keyboard (at 1kHz) gives about 30-50 drawn frames per second. Demos are slower 10-30.

Some of my code (anti-ghosting, Gui, demos) are adopted and were already present in my [fork of kiibohd](https://github.com/cryham/controller),
done for my previous keyboards, info on [My Website](http://cryham.tuxfamily.org/archives/portfolio/crystal-keyboard-3-and-4).

Hardware
--------

The keyboard is made of:
* Teensy 3.2 (or 3.1), overclocked, stable at 144 MHz
* TFT LCD display ST7735, 160 x 128, 16bit color RGB565. Red PCB with SD card slot. Terribly low viewing angle.
* NPN transistor BC550. Connection from DAC A14 output through 4.7k resistor to base, collector to 3.3V, emitter through 47 ohm resistor to Display LED.

Licensing
---------

My sources are licensed GPLv3. For other sources used, see their repositories.
