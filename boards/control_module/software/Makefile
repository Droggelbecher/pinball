
TARGET=pinball

CXXFLAGS = -std=c++11 -Wall
CXXFLAGS += -I.
CXXFLAGS += -I../../../common_software/include/
CXXFLAGS += -g -O0

CXX = g++
LD = g++

SRC=main.cc gamelogic.cc

OBJDIR=obj
OBJS=$(SRC:%.cc=$(OBJDIR)/%.o)

$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $@

$(OBJDIR)/%.o :%.cc
	$(CXX) -c $(CXXFLAGS) $< -o $@

# Create object files directory
$(shell mkdir $(OBJDIR) 2>/dev/null)

# Include the dependency files.
-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)


