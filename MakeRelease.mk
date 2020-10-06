CXX       := g++
CXX_FLAGS := -Wall -std=c++11 -MMD -MP -DUNICODE -D_UNICODE -O2 -DNDEBUG
LD_FLAGS := -lgdi32 -ldwmapi -lmsimg32 -lpng -lcomctl32 -lz -s --static -Wl,-subsystem,windows

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

$(BIN)/$(EXECUTABLE): $(OBJECTS) $(OBJ)/deskcal.res | $(BIN)
	$(CXX) $(filter-out $(BIN), $^) -o $@ $(LD_FLAGS)

$(OBJ)/%.o : $(SRC)/%.cpp | $(OBJ)
	$(CXX) $(CXX_FLAGS) -I$(INC) -c -o $@ $<

$(OBJ)/%.res : %.rc | $(OBJ)
	windres.exe -i $< -J rc -o $@ -O coff

$(BIN):
	-mkdir -p $(BIN)

$(OBJ):
	-mkdir -p $(OBJ)

clean:
	-rm $(BIN)/$(EXECUTABLE) $(OBJ)/*

-include $(DEPENDS)