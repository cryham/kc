Keyboard Controler
==================

This my own USB Keyboard Controler code for Teensy 3.2 with TFT ST7735 color display.

Called K.C. ("Kacey") for short.

It allows editing everything on keyboard display, in real time. No other tools needed.

More description of project on [My Website](http://cryham.tuxfamily.org/portfolio/2018-k-c-controller), with links to [videos](https://www.youtube.com/channel/UC9-gc9xgEeuSSKB8_dESLGw/videos).

### Screenshots

![](https://raw.githubusercontent.com/cryham/kc/master/screens.jpg)

## Features

* **Mapping** - menu, for remapping USB key codes to physical keys.
* **Layers**
* **Sequences** - editor for macros, passwords, and more.
* In sequence **commands** - wait, set key delay, comment part, hide rest, repeat itself, run other sequence, and all mouse commands.
* Visual keyboard - drawn on display with current mappings.
* **Mouse** keys - with move acceleration, also for wheel.

Also:
* Setup in EEPROM - loading and saving full keyboard setup (parameters, mappings, sequences).
* Anti-ghosting code - needed for foil keyboards without diodes.
* Testing pages - showing pressed keys info, matrix ghosting, layer use, etc.
* Keys list - for picking, with custom group colors and filtering.
* Parameters - for Scan Setup, Mouse and Display, adjustable in menu.
* Sequence preview in Mapping and Testing views. Also Sequence view lists mapped keys (if any).

Lastly:
* Many **Demos** with presets. E.g.: Plasma, Wave, Fire, Polyhedrons 3D, waving 2D CK logo.
* Falling blocks **Game** - with random generated blocks, many presets and custom options.
* **Clock** with date (needs 3V battery).
* **Temperature** reading from DS18B20 sensor (optional, 1pin).
* LED light (optional, 1pin).
* **Statistics**: uptime, key presses (per minute and average), keyboard inactive and active (last) times. Also for used keys, layers, sequences etc.
* 2 **Graphs**: key presses per minute and temperature'C (auto ranged).
* **Help** pages with all GUI keys used and commands listed.
* Colorful **GUI Menu** with 2 levels and 3 fonts (2 my own).

---

### Code used

It uses code from repositories, quite reduced and/or modified:
* [PaulStoffregen cores](https://github.com/PaulStoffregen/cores/tree/master/teensy3) for Teensy 3 core.
* [sumotoy TFT_ST7735](https://github.com/sumotoy/TFT_ST7735/tree/1.0p1) for fast TFT ST7735 display.
* [Adafruit-GFX-Library](https://github.com/adafruit/Adafruit-GFX-Library) for drawing, combined with above.
* [Kiibohd](https://github.com/kiibohd/controller) only keyboard matrix scan code.

Since I am using a buffer (40kB for 160x128), all drawing code fills it. On each frame, buffer gets sent to display, thus no flickering.
All unnecessary code for drawing from libraries was removed.

Displaying GUI and scanning keyboard (at 1kHz) gives about 30-50 drawn frames per second. Demos are slower 10-30.

Some of my code (anti-ghosting, Gui, demos) are adopted and were already present in my [fork of kiibohd](https://github.com/cryham/controller),
done for my previous keyboards, info also on [my website](http://cryham.tuxfamily.org/portfolio/crystal-keyboard-3-and-4).

#### Licensing

My sources are licensed GPLv3. For other sources used, see their repositories.

---

### Hardware

The keyboard is made of:
* Teensy 3.2 (or 3.1), overclocked, stable at 144 MHz (or 120 MHz, with longer wires).
* TFT LCD display ST7735, 160 x 128, 16bit color RGB565. Red PCB with SD card slot. Terribly low horizontal viewing angle.
* NPN transistor BC550. Connection from DAC A14 output through 4.7k resistor to base, collector to 3.3V, emitter through 47 (or less) ohm resistor to Display LED.
* Optionally a DS18B20 1-wire temperature sensor with data pin through 4.7k resistor to 3.3V.

Display uses pins: 9 DC, 10 CS, 26 RST (set in `Ada4_ST7735.cpp`) 11 DOUT, 13 CLK and 40 DAC for backlight LED.

### Configuring

Setup is done in files:
* `def.h`
  * Define code features to use like: demos, game. Also optional pins for LED and Temp'C.
  * Choose keyboard type by define (CK1,CK6,CK7 or create own).
* `matrix.h` has defined Teensy pins for keyboard Matrix_cols and Matrix_rows.
  * CK1 is easiest for testing (8 cols x 6 rows). CK6 and CK7 use 18 cols and 8 rows (same pins).
  * Columns are as outputs (all High-Z with one set to low "0"), while rows are inputs (all high "1").
* `kbd_layout.cpp` has physical keys layout.

Other key constants are in `kc_params.h` like (max) counts of rows, cols, layers, sequences (also updated in `keys_usb.h`).

When defining a new keyboard, it is useful to force starting in main.cpp on GUI Pressed tab, it shows scan codes.

### Building

On **Windows** it is done using [Cygwin](https://www.cygwin.com/).  
Needs to have arm-gnu-none-eabi installed, just like for Linux.

On **Linux** required package are (at least): binutils-arm-gnu-none-eabi, gcc-arm-gnu-none-eabi, git.  
On Debian based, you can `sudo apt-get install` them or use package manager.


To get sources in console use:
```
git clone https://github.com/cryham/kc.git
```
Then `cd kc` and `make` (or `make -j8` for 8 threads/CPUs) to start building.

On successful build the last lines are e.g.
```
  CC kc/matrix.c
 Linking 
   SRAM: 75%  49472 / 65536 B
  Flash: 84%  221416 / 262144 B
```
showing used percentages of memories.

The output file `main.hex` is created inside `bin/` folder.  
Open it in [Teensy Loader App](https://www.pjrc.com/teensy/loader.html) and press reset on board to flash (upload) and use.
