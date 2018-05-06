# Teensy 3.2
MCU = MK20DX256
LOWER_MCU = mk20dx256
CPUARCH = cortex-m4

# project name for hex file
TARGET = main


#  configurable options  + + +
#************************************************************************
#OPTIONS = -DF_CPU=144000000  # CK6/3
OPTIONS = -DF_CPU=120000000  # CK7/4

# usb_desc.h : USB_HID  USB_KEYBOARDONLY 
OPTIONS += -DUSB_HID -DLAYOUT_US_ENGLISH -DUSING_MAKEFILE

# options needed by many Arduino libraries to configure for Teensy 3.x
OPTIONS += -D__$(MCU)__ -DARDUINO=10805 -DTEENSYDUINO=141


#  PATHS
#************************************************************************
# cygwin64
COMPILERPATH ?= /usr/local/bin

# source subdirs 3
SRCDIR = t3
SRCLIB = lib
SRCKC = kc

# output dirs
OBJDIR = obj
BINDIR = bin
PROJECT = main

INC = -I$(SRCDIR) -I$(SRCLIB) -I$(SRCKC)
MCU_LD = $(SRCDIR)/$(LOWER_MCU).ld


#  FLAGS
#************************************************************************

ARCH = -mcpu=$(CPUARCH) -mthumb -MMD
WARN = -Wall -g
OPT = -O3

# compiler options for C only
CFLAGS = $(WARN) $(OPT) $(ARCH) $(OPTIONS)

# compiler options for C++ only
CXXFLAGS = $(WARN) $(OPT) $(ARCH) -std=gnu++14 -felide-constructors -fno-exceptions -fno-rtti $(OPTIONS)

# linker options
LDFLAGS = -Os -Wl,--gc-sections,--defsym=__rtc_localtime=0 --specs=nano.specs -mcpu=$(CPUARCH) -mthumb -T$(MCU_LD)
LIBS = -lm -lstdc++


#  compiler programs
CC_ = $(COMPILERPATH)/arm-none-eabi-gcc
CXX_ = $(COMPILERPATH)/arm-none-eabi-g++
CC = @$(CC_)
CXX = @$(CC_)
OBJCOPY = @$(COMPILERPATH)/arm-none-eabi-objcopy
SIZE = $(COMPILERPATH)/arm-none-eabi-size

#  auto create lists of sources and objects
C_FILES := $(wildcard $(SRCDIR)/*.c) $(wildcard $(SRCLIB)/*.c) $(wildcard $(SRCKC)/*.c)
CPP_FILES := $(wildcard $(SRCDIR)/*.cpp) $(wildcard $(SRCLIB)/*.cpp) $(wildcard $(SRCKC)/*.cpp)
OBJ_FILES := $(addprefix $(OBJDIR)/,$(notdir $(CPP_FILES:.cpp=.o))) $(addprefix $(OBJDIR)/,$(notdir $(C_FILES:.c=.o)))


#  util, color errors and warnings
#************************************************************************

#  "\e[38;5;27m   21 57v  63bv  27 33 39 45 bl  212 pink
#		yl 226 220  or 166  196 red  40 grn  51 cy   90 154 ylgrn
#	@mkdir -p $(dir $@) ?

CC_CLR  = \e[38;5;33m
CXX_CLR = \e[38;5;39m

ERR_CLR  = \e[38;5;202m
WARN_CLR = \e[33m

OBJ_CLR  = \e[38;5;248m
NORM_CLR = \e[38;5;249m
NO_CLR   = \033[m
ST_CLR = \e[38;5;51m

COLOR_OUTPUT = 2>&1 |                                   \
	while IFS='' read -r line; do                       \
		if  [[ $$line == *:[\ ]error:* ]]; then         \
			echo -e "$(ERR_CLR)$${line}$(NO_CLR)";     \
		elif [[ $$line == *:[\ ]warning:* ]]; then      \
			echo -e "$(WARN_CLR)$${line}$(NO_CLR)";     \
		else                                            \
			echo -e "$(NORM_CLR)$${line}$(NO_CLR)";     \
		fi;                                             \
	done; exit $${PIPESTATUS[0]};


#  BUILD
#************************************************************************

kc: $(BINDIR)/$(PROJECT).hex

# C compilation
$(OBJDIR)/%.o : $(SRCKC)/%.c
	@echo -e "$(CC_CLR)  CC\e[m" $<
	$(CC) $(CFLAGS) $(INC) -c $< -o $@ $(COLOR_OUTPUT)
$(OBJDIR)/%.o : $(SRCLIB)/%.c
	@echo -e "$(CC_CLR)  CC\e[m" $<
	$(CC) $(CFLAGS) $(INC) -c $< -o $@ $(COLOR_OUTPUT)
$(OBJDIR)/%.o : $(SRCDIR)/%.c
	@echo -e "$(CC_CLR)  CC\e[m" $<
	$(CC) $(CFLAGS) $(INC) -c $< -o $@ $(COLOR_OUTPUT)

# C++ compilation
$(OBJDIR)/%.o : $(SRCKC)/%.cpp
	@echo -e "$(CXX_CLR) CXX\e[m" $<
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@ $(COLOR_OUTPUT)
$(OBJDIR)/%.o : $(SRCLIB)/%.cpp
	@echo -e "$(CXX_CLR) CXX\e[m" $<
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@ $(COLOR_OUTPUT)
$(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	@echo -e "$(CXX_CLR) CXX\e[m" $<
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@ $(COLOR_OUTPUT)


# Linker invocation
$(BINDIR)/$(PROJECT).elf: $(OBJ_FILES) $(MCU_LD)
	@echo -e "\e[38;5;154m Linking \e[m"
	$(CC) $(LDFLAGS) -o $@ $(OBJ_FILES) $(LIBS)


# Hex, last
$(BINDIR)/$(PROJECT).hex : $(BINDIR)/$(PROJECT).elf
#	$(SIZE) $<
	@./sizeCalc $(SIZE) ram $< 65536 " SRAM"
	@./sizeCalc $(SIZE) flash $< 262144 Flash
	$(OBJCOPY) -O ihex -R .eeprom $< $@


#  SHORTCUTS
#************************************************************************

#clean:
c:
	@echo -e "$(ST_CLR)Clean$(NO_CLR)"
	rm -f $(OBJDIR)/*.o $(OBJDIR)/*.d $(BINDIR)/$(PROJECT).elf $(BINDIR)/$(PROJECT).hex
#rebuild:
r:
	@make c --no-print-directory
	@echo -e "$(ST_CLR)Rebuild$(NO_CLR)"
	@make m --no-print-directory
#make -j
m:
	@make -j --no-print-directory
