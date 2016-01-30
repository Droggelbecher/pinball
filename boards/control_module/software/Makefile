
TARGET=pinball

OBJDIR = obj
ADD_SOURCE_DIR = ../../../common_software/src

SOURCES = game_logic.cc main.cc \
	solenoids.cc lamps.cc switches.cc display.cc \
	pcf_font.cc \
	audio.cc

CXXFLAGS = -std=c++14 -Wall
CXXFLAGS += -I.
CXXFLAGS += -I../../../common_software/include/
CXXFLAGS += -g -O0

LDFLAGS = -lopenal -lalure
LDFLAGS += -g -O0

CC = gcc
CXXC = g++
LD = g++

OBJECTS = $(SOURCES:%.cc=$(OBJDIR)/%.o)

# Compiler flags to generate dependency files.
GENDEPFLAGS = -MD -MP -MF .dep/$(@F).d
CFLAGS += $(GENDEPFLAGS)
CXXFLAGS += $(GENDEPFLAGS)

.PHONY: all clean deploy

all: $(TARGET)

clean:
	rm $(OBJDIR)/* || :


$(TARGET): $(OBJECTS)
	$(LD) $(LDFLAGS) $(OBJECTS) -o $@

$(OBJDIR)/%.o: %.cc
	$(CXXC) -c $(CXXFLAGS) $< -o $@

# Create object files directory
$(shell mkdir $(OBJDIR) 2>/dev/null)

# Include the dependency files.
-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)

