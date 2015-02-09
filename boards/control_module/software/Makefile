
TARGET = pinball_controller
OBJDIR = obj

SOURCES_CONTROLLER = gpio.c main.c spi.c spi_ss.c display.c
SOURCES_SPI_DEBUG = gpio.c spi_debug.c spi.c spi_ss.c display.c
#HEADERS = config.h gpio.h spi.h spi_ss.h

CFLAGS = -Wall -Wundef -Wstrict-prototypes
CFLAGS += -I.
CFLAGS += -g
LDFLAGS =

DEPLOY_HOST = raspberrypi
DEPLOY_USER = pi
DEPLOY_DIR = /tmp/pinball


CC = gcc
LD = ld

OBJECTS_CONTROLLER = $(SOURCES_CONTROLLER:%.c=$(OBJDIR)/%.o)
OBJECTS_SPI_DEBUG = $(SOURCES_SPI_DEBUG:%.c=$(OBJDIR)/%.o)

# Compiler flags to generate dependency files.
GENDEPFLAGS = -MD -MP -MF .dep/$(@F).d
CFLAGS += $(GENDEPFLAGS)


.PHONY: all clean deploy

all: $(TARGET)

clean:
	rm $(OBJDIR)/* || :

deploy: clean
	ssh $(DEPLOY_USER)@$(DEPLOY_HOST) "sudo killall $(TARGET); mkdir -p $(DEPLOY_DIR)"
	ssh $(DEPLOY_USER)@$(DEPLOY_HOST) "sudo killall spi_debug; mkdir -p $(DEPLOY_DIR)"
	rsync -ChaPx ./ $(DEPLOY_USER)@$(DEPLOY_HOST):$(DEPLOY_DIR)
	ssh $(DEPLOY_USER)@$(DEPLOY_HOST) "cd $(DEPLOY_DIR); make; sudo ./$(TARGET)"

deploy_debug: clean
	ssh $(DEPLOY_USER)@$(DEPLOY_HOST) "sudo killall $(TARGET); mkdir -p $(DEPLOY_DIR)"
	ssh $(DEPLOY_USER)@$(DEPLOY_HOST) "sudo killall spi_debug; mkdir -p $(DEPLOY_DIR)"
	rsync -ChaPx ./ $(DEPLOY_USER)@$(DEPLOY_HOST):$(DEPLOY_DIR)
	ssh $(DEPLOY_USER)@$(DEPLOY_HOST) "cd $(DEPLOY_DIR); make spi_debug; sudo ./spi_debug"

$(TARGET): $(OBJECTS_CONTROLLER)
	$(CC) $(LDFLAGS) $(OBJECTS_CONTROLLER) -o $@

spi_debug: $(OBJECTS_SPI_DEBUG)
	$(CC) $(LDFLAGS) $(OBJECTS_SPI_DEBUG) -o $@

$(OBJDIR)/%.o :%.c
	$(CC) -c $(CFLAGS) $< -o $@


# Create object files directory
$(shell mkdir $(OBJDIR) 2>/dev/null)

# Include the dependency files.
-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)


