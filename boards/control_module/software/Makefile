
TARGET=pinball

#DEPLOY_HOST = raspberrypi
DEPLOY_HOST = 10.0.0.42
DEPLOY_USER = pinball
DEPLOY_DIR = /home/pinball/pinball
DEPLOY_SCRIPT_DIR = /home/pinball

OBJDIR = $(abspath obj)
CXX_SOURCES = \
	game_logic.cc \
	audio.cc \
	main.cc \
	spi.cc \
	switches.cc \
	display_canvas.cc \
	utils.cc \
	canvas_buffer.cc \
	canvas.cc \
	framer.cc

	#scrolling_canvas.cc \
	#pcf_font.cc 

C_SOURCES = checksum.c

CXXFLAGS = -std=c++11 -Wall
CXXFLAGS += -I.
CXXFLAGS += -g -O0

CFLAGS += -I.
CFLAGS += -g -O0

LDFLAGS = -lopenal -lalure
LDFLAGS += -g -O0

CC = gcc
CXXC = g++
LD = g++

CXX_OBJECTS = $(CXX_SOURCES:%.cc=$(OBJDIR)/%.o)
C_OBJECTS = $(C_SOURCES:%.c=$(OBJDIR)/%.o)
OBJECTS = $(CXX_OBJECTS) $(C_OBJECTS)

# Compiler flags to generate dependency files.
GENDEPFLAGS = -MD -MP -MF .dep/$(@F).d
CFLAGS += $(GENDEPFLAGS)
CXXFLAGS += $(GENDEPFLAGS)

.PHONY: all clean deploy

all: $(TARGET)

deploy: clean
	ssh $(DEPLOY_USER)@$(DEPLOY_HOST) "sudo killall $(TARGET); mkdir -p $(DEPLOY_DIR)"
	#ssh $(DEPLOY_USER)@$(DEPLOY_HOST) "killall spi_debug; mkdir -p $(DEPLOY_DIR)"
	rsync -CHaPx ../../../ $(DEPLOY_USER)@$(DEPLOY_HOST):$(DEPLOY_DIR)/ --exclude datasheets/ --exclude casing/ --delete --exclude obj/ --exclude pinball_controller
	rsync -CHaPx ./system/check_compile.sh $(DEPLOY_USER)@$(DEPLOY_HOST):$(DEPLOY_SCRIPT_DIR)/check_compile.sh
	#ssh $(DEPLOY_USER)@$(DEPLOY_HOST) "cd $(DEPLOY_DIR)/boards/control_module/software/"
	#ssh $(DEPLOY_USER)@$(DEPLOY_HOST) "$(DEPLOY_SCRIPT_DIR)/check_compile.sh"

	#rsync -ChaPx ./ $(DEPLOY_USER)@$(DEPLOY_HOST):$(DEPLOY_DIR)
	#rsync -ChaPx ../../../common_software/include/ $(DEPLOY_USER)@$(DEPLOY_HOST):$(DEPLOY_DIR)
	#rsync -ChaPx ../../../common_software/src/ $(DEPLOY_USER)@$(DEPLOY_HOST):$(DEPLOY_DIR)

clean:
	rm $(TARGET) $(OBJDIR)/* || :


$(TARGET): $(OBJECTS) 
	$(LD) $(LDFLAGS) $(OBJECTS) -o $@

$(OBJDIR)/%.o: %.cc
	$(CXXC) -c $(CXXFLAGS) $< -o $@

$(OBJDIR)/%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

$(OBJDIR)/checksum.o: ../../../common_software/src/checksum.c
	$(CC) -c $(CFLAGS) $< -o $@

# Create object files directory
$(shell mkdir $(OBJDIR) 2>/dev/null)

# Include the dependency files.
-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)

