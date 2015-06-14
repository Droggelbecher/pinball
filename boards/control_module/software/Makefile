
TARGET_PLATFORM = pi

ifeq ($(TARGET_PLATFORM),pi)
 USE_DEBUG_DISPLAY = 0
 USE_DEBUG_SPI = 0
 USE_DEBUG_GPIO = 0
 DEBUGGING = 0
else
 USE_DEBUG_DISPLAY = 1
 USE_DEBUG_SPI = 1
 USE_DEBUG_GPIO = 1
 DEBUGGING = 1
endif

TARGET = pinball_controller
OBJDIR = obj
ADDSRCDIR = ../../../common_software/src

SOURCES_CONTROLLER = main.c spi_ss.c render.c gamelogic.c solenoids.c switches.c checksum.c pcf.c scroll.c utils.c audio.c
SOURCES_SPI_DEBUG = spi_debug.c spi_ss.c render.c linux_spi.c raspberry_gpio.c spi_display.c 

#HEADERS = config.h gpio.h spi.h spi_ss.h

CFLAGS = -Wall -Wundef -Wstrict-prototypes
CFLAGS += -I.
CFLAGS += -I../../../common_software/include/

#CFLAGS += -lopenal -lalure

ifeq ($(DEBUGGING),1)
 CFLAGS += -g -O0
 LDFLAGS += -g -O0
else
 CFLAGS += -O3
 LDFLAGS += -O3
endif

CFLAGS += -DUSE_DEBUG_DISPLAY=$(USE_DEBUG_DISPLAY)
CFLAGS += -DUSE_DEBUG_SPI=$(USE_DEBUG_SPI)
CFLAGS += -DUSE_DEBUG_GPIO=$(USE_DEBUG_GPIO)

ifeq ($(USE_DEBUG_DISPLAY),1)
 SOURCES_CONTROLLER += debug_display.c
else
 SOURCES_CONTROLLER += spi_display.c
endif

ifeq ($(USE_DEBUG_SPI),1)
 SOURCES_CONTROLLER += debug_spi.c
else
 SOURCES_CONTROLLER += linux_spi.c
endif

ifeq ($(USE_DEBUG_GPIO),1)
 SOURCES_CONTROLLER += debug_gpio.c
else
 SOURCES_CONTROLLER += raspberry_gpio.c
endif


LDFLAGS += -lopenal -lalure

DEPLOY_HOST = raspberrypi
DEPLOY_USER = pinball
DEPLOY_DIR = /home/pinball/pinball
DEPLOY_SCRIPT_DIR = /home/pinball


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
	ssh $(DEPLOY_USER)@$(DEPLOY_HOST) "killall spi_debug; mkdir -p $(DEPLOY_DIR)"
	rsync -CHaPx ../../../ $(DEPLOY_USER)@$(DEPLOY_HOST):$(DEPLOY_DIR)/ --exclude datasheets/ --exclude casing/ --exclude resources/ --delete --exclude obj/ --exclude pinball_controller
	rsync -CHaPx ./system/check_compile.sh $(DEPLOY_USER)@$(DEPLOY_HOST):$(DEPLOY_SCRIPT_DIR)/check_compile.sh
	#ssh $(DEPLOY_USER)@$(DEPLOY_HOST) "cd $(DEPLOY_DIR)/boards/control_module/software/"
	ssh $(DEPLOY_USER)@$(DEPLOY_HOST) "$(DEPLOY_SCRIPT_DIR)/check_compile.sh"

	#rsync -ChaPx ./ $(DEPLOY_USER)@$(DEPLOY_HOST):$(DEPLOY_DIR)
	#rsync -ChaPx ../../../common_software/include/ $(DEPLOY_USER)@$(DEPLOY_HOST):$(DEPLOY_DIR)
	#rsync -ChaPx ../../../common_software/src/ $(DEPLOY_USER)@$(DEPLOY_HOST):$(DEPLOY_DIR)

deploy_debug: clean
	ssh $(DEPLOY_USER)@$(DEPLOY_HOST) "sudo killall $(TARGET); mkdir -p $(DEPLOY_DIR)"
	ssh $(DEPLOY_USER)@$(DEPLOY_HOST) "sudo killall spi_debug; mkdir -p $(DEPLOY_DIR)"
	rsync -ChaPx ./ $(DEPLOY_USER)@$(DEPLOY_HOST):$(DEPLOY_DIR)
	ssh $(DEPLOY_USER)@$(DEPLOY_HOST) "cd $(DEPLOY_DIR); make spi_debug; sudo ./spi_debug"

$(TARGET): $(OBJECTS_CONTROLLER)
	$(CC) $(LDFLAGS) $(OBJECTS_CONTROLLER) -o $@

test_alure: test_alure.o audio.o
	$(CC) $(LDFLAGS) test_alure.o audio.o -o $@

spi_debug: $(OBJECTS_SPI_DEBUG)
	$(CC) $(LDFLAGS) $(OBJECTS_SPI_DEBUG) -o $@

$(OBJDIR)/%.o :%.c
	$(CC) -c $(CFLAGS) $< -o $@

$(OBJDIR)/%.o :$(ADDSRCDIR)/%.c
	$(CC) -c $(CFLAGS) $< -o $@

# Create object files directory
$(shell mkdir $(OBJDIR) 2>/dev/null)

# Include the dependency files.
-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)


