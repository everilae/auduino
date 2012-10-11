# Base directories for Arduino
AVR_TOOLCHAIN	= /opt/cross/avr/lib64/gcc/avr/4.7.0
ARDUINO_BASE	= /usr/share/arduino-1.0.1/hardware/arduino
ARDUINO_CORE	= $(ARDUINO_BASE)/cores/arduino
ARDUINO_VARIANT	= $(ARDUINO_BASE)/variants/standard

# Fix PATH for correct toolchain
PATH	:= $(AVR_TOOLCHAIN)/bin:$(PATH)

# Explicit tools
ifeq ($(DEBUG),)
PRINT	= printf
ERROUT	= 2>/dev/null
CC	= @$(PRINT) "(CC)  $@\n"; avr-gcc $(ERROUT)
CXX	= @$(PRINT) "(CXX) $@\n"; avr-g++ $(ERROUT)
AS	= @$(PRINT) "(AS)  $@\n"; avr-as  $(ERROUT)
AR	= @$(PRINT) "(AR)  $@\n"; avr-ar  $(ERROUT)
LD	= @$(PRINT) "(LD)  $@\n"; avr-ld  $(ERROUT)
else
CC	= avr-gcc
CXX	= avr-g++
AS	= avr-as
AR	= avr-ar
LD	= avr-ld
endif

SUDO	= sudo

# Unfortunately different tools have different names for same mcu
ifeq ($(MCU),)
MCU	= atmega328p
endif
ifeq ($(SIMMCU),)
SIMMCU	= atmega328
endif
ifeq ($(ISPMCU),)
ISPMCU	= m328p
endif

# Frequency, for avr/io.h and such
F_CPU	= 16000000L
ARDUINO	= 101

# Build directories
INCDIR	= include
SRCDIR  = src
OBJDIR	= obj
LIBDIR	= lib

CDEF	= -DF_CPU=$(F_CPU) -DARDUINO=$(ARDUINO) -DUSB_VID=null -DUSB_PID=null
CINC	= -I$(ARDUINO_VARIANT) -I$(ARDUINO_CORE) -I$(INCDIR)
CLIB    = -L$(LIBDIR)

ifneq ($(DEBUG),)
CDEF	+= -DDEBUG=1
endif

ifneq ($(DEBUG),)
CDEBUG	= -g
endif
CWARN	= -Wall -Winline
CSTD	= -std=gnu1x
CXXSTD	= -std=gnu++11
COPTS	= -Os -ffunction-sections -fdata-sections

CFLAGS		= $(CDEBUG) $(CWARN) $(CSTD)   $(COPTS) -pedantic -mmcu=$(MCU) $(CDEF) $(CINC)
CXXFLAGS	= $(CDEBUG) $(CWARN) $(CXXSTD) $(COPTS) -pedantic -mmcu=$(MCU) $(CDEF) $(CINC) \
		  -fno-exceptions -felide-constructors
LDFLAGS		= $(CDEBUG) $(CWARN) -Wl,--gc-sections -mmcu=$(MCU) $(CLIB)
LIBRARIES	= -lm

O2HEX		= avr-objcopy -O ihex
O2HEX_T		= $(O2HEX) -j .text -j .data
OBJDUMP		= avr-objdump
OBJCOPY		= avr-objcopy -v
AVRSIZE		= avr-size
AVRSTRIP	= avr-strip
AVRGDB		= avr-gdbtui
ifeq ($(DEBUG),)
AVRISP		= @avrdude
SIMULATOR	= @simulavr
else
AVRISP		= avrdude -v -v -v -v
SIMULATOR	= simulavr --verbose
endif

SIMWPIPE	= 0x20,-
SIMRPIPE	= 0x22,-
SIMARGS		= --device $(SIMMCU) \
		--file $(TARGET) \
		--cpufrequency $(F_CPU) \
		--writetopipe $(SIMWPIPE) \
		--readfrompipe $(SIMRPIPE)

ISPPORT		= /dev/ttyUSB0
ISPBAUDRATE	= 115200
ISPARGS		= -P $(ISPPORT) \
		-b $(ISPBAUDRATE) \
		-p $(ISPMCU) \
		-c arduino \
		-F \
		-e \
		-U flash:w:
ISP		= $(AVRISP) $(ISPARGS)$1

vpath %.c	$(SRCDIR):$(ARDUINO_CORE)
vpath %.cpp	$(SRCDIR):$(ARDUINO_CORE)
vpath %.h	$(INCDIR)
vpath %.o	$(OBJDIR)
vpath %.a	$(LIBDIR)

CORECSOURCES	=  $(notdir $(wildcard $(ARDUINO_CORE)/*.c))
CORECXXSOURCES	=  $(notdir $(wildcard $(ARDUINO_CORE)/*.cpp))
COREOBJ		=  $(CORECSOURCES:%.c=$(OBJDIR)/%.o)
COREOBJ		+= $(CORECXXSOURCES:%.cpp=$(OBJDIR)/%.o)
LIBRARIES	+= -lcore

TARGETOBJ	= $(TARGET).o midi.o
TARGET		= auduino

.PHONY: all

all: $(OBJDIR) $(LIBDIR) $(TARGET)

ifneq ($(DEBUG),)
TARGETOBJ	+= debug.o
endif

$(TARGET): $(LIBDIR)/libcore.a
$(TARGET): $(TARGETOBJ:%=$(OBJDIR)/%)
	$(CC) $(LDFLAGS) $(TARGETOBJ:%=$(OBJDIR)/%) $(LIBRARIES) -o $@

.PHONY: clean

clean::
	$(RM) $(TARGET)

$(TARGET).hex: $(TARGET)
	$(O2HEX_T) $^ $@

clean::
	$(RM) $(TARGET).hex

$(LIBDIR)/libcore.a : $(COREOBJ)
	$(AR) rcs $@ $^

clean::
	$(RM) $(LIBDIR)/libcore.a

$(OBJDIR)/%.o : %.c
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -o $@ $<

$(OBJDIR)/%.o : %.cpp
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) -o $@ $<

clean::
	$(RM) $(OBJDIR)/*.o
	
$(OBJDIR):
	mkdir $(OBJDIR)

$(LIBDIR):
	mkdir $(LIBDIR)

.PHONY: upload

upload: $(TARGET).hex
	$(call ISP,$<)

.PHONY: install

install: upload

.PHONY: strip

strip: $(TARGET)
	$(AVRSTRIP) $(TARGET)

.PHONY: size

size: $(TARGET)
	$(AVRSIZE) $(TARGET)

.PHONY: simulate

simulate: $(TARGET)
	$(SIMULATOR) $(SIMARGS)

.PHONY: gdbserver

gdbserver: $(TARGET)
	$(SIMULATOR) $(SIMARGS) --gdbserver
