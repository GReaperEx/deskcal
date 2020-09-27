CXX       := g++
CXX_FLAGS := -Wall -std=c++11 -MMD -MP -DUNICODE -D_UNICODE -O2 -DNDEBUG
LD_FLAGS := -ldwmapi -lmsimg32 -lgdi32 -s

BIN := bin/Release
OBJ	:= obj/Release
INC := include
SRC := source

EXECUTABLE := deskcal.exe

SOURCES := $(wildcard $(SRC)/*.cpp)
OBJECTS := $(patsubst $(SRC)/%.cpp,$(OBJ)/%.o,$(SOURCES))
DEPENDS := $(OBJECTS:.o=.d)

.PHONY: all clean

all: $(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(OBJECTS) | $(BIN)
	$(CXX) $(filter-out $(BIN), $^) -o $@ $(LD_FLAGS)

$(OBJ)/%.o : $(SRC)/%.cpp | $(OBJ)
	$(CXX) $(CXX_FLAGS) -I$(INC) -c -o $@ $<

$(BIN):
	-mkdir -p $(BIN)

$(OBJ):
	-mkdir -p $(OBJ)

clean:
	-rm $(BIN)/$(EXECUTABLE) $(OBJ)/*

-include $(DEPENDS)