
TARGET = pinball_controller
OBJDIR = obj

SOURCES = gpio.c main.c spi.c spi_ss.c
#HEADERS = config.h gpio.h spi.h spi_ss.h

CFLAGS = -Wall -Wundef -Wstrict-prototypes
CFLAGS += -I.
LDFLAGS =


CC = gcc
LD = ld

OBJECTS = $(SOURCES:%.c=$(OBJDIR)/%.o)

# Compiler flags to generate dependency files.
GENDEPFLAGS = -MD -MP -MF .dep/$(@F).d
CFLAGS += $(GENDEPFLAGS)


.PHONY: all clean

all: $(TARGET)

clean:
	rm $(OBJDIR)/*

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

$(OBJDIR)/%.o :%.c
	$(CC) -c $(CFLAGS) $< -o $@


# Create object files directory
$(shell mkdir $(OBJDIR) 2>/dev/null)

# Include the dependency files.
-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)


