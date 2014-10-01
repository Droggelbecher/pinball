
TARGET = pinball_controller
OBJDIR = obj

SOURCES = gpio.c main.c spi.c spi_ss.c display.c
#HEADERS = config.h gpio.h spi.h spi_ss.h

CFLAGS = -Wall -Wundef -Wstrict-prototypes
CFLAGS += -I.
CFLAGS += -g
LDFLAGS =

DEPLOY_HOST = data
DEPLOY_USER = pi
DEPLOY_DIR = /tmp/pinball


CC = gcc
LD = ld

OBJECTS = $(SOURCES:%.c=$(OBJDIR)/%.o)

# Compiler flags to generate dependency files.
GENDEPFLAGS = -MD -MP -MF .dep/$(@F).d
CFLAGS += $(GENDEPFLAGS)


.PHONY: all clean deploy

all: $(TARGET)

clean:
	rm $(OBJDIR)/* || :

deploy: clean
	ssh $(DEPLOY_USER)@$(DEPLOY_HOST) "sudo killall $(TARGET); mkdir -p $(DEPLOY_DIR)"
	rsync -ChaPx ./ $(DEPLOY_USER)@$(DEPLOY_HOST):$(DEPLOY_DIR)
	ssh $(DEPLOY_USER)@$(DEPLOY_HOST) "cd $(DEPLOY_DIR); make; sudo ./$(TARGET)"

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

$(OBJDIR)/%.o :%.c
	$(CC) -c $(CFLAGS) $< -o $@


# Create object files directory
$(shell mkdir $(OBJDIR) 2>/dev/null)

# Include the dependency files.
-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)


