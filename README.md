[Auduino+MIDI](http://github.com/everilae/auduino/)
===================================================

Auduino, the Lo-Fi granular synthesiser by Peter Knight, Tinker.it http://tinker.it.

* Help: http://code.google.com/p/tinkerit/wiki/Auduino
* More help: http://groups.google.com/group/auduino

Midi additions and C++ support by Ilja Everilä <saarni@gmail.com>.

Todo
----

Add autoconf or some such method for setting up Makefile variables.

Quick start
-----------

Clone the repo, `git clone git://github.com/everilae/auduino.git`, then run:

```
cd auduino
make
```

Until autotools is up and running you have to set some variables in the Makefile to get things working:

```
# Base directories for Arduino
AVR_TOOLCHAIN	= /opt/cross/avr/lib64/gcc/avr/4.7.0
ARDUINO_BASE	= /usr/share/arduino-1.0.1/hardware/arduino
ARDUINO_CORE	= $(ARDUINO_BASE)/cores/arduino
ARDUINO_VARIANT	= $(ARDUINO_BASE)/variants/standard

# Fix PATH for correct toolchain
PATH	:= $(AVR_TOOLCHAIN)/bin:$(PATH)
```

Also set MCU variables (or provide them at the command line):

```
MCU	= atmega328p
SIMMCU	= atmega328
ISPMCU	= m328p
```

If your processor frequency differs from 16MHz, set it with:

```
F_CPU	= 16000000L
```

Building with debugging
-----------------------

```
make DEBUG=1
```

Uploading to device
-------------------

```
make upload
```
