CXX       := g++
CXX_FLAGS := -Wall -std=c++11 -ggdb -MMD -MP -D_DEBUG -DUNICODE -D_UNICODE

BIN := bin/Debug
OBJ	:= obj/Debug
INC := include
SRC := source

LIBRARIES  := -ldwmapi -lmsimg32 -lgdi32
EXECUTABLE := deskcal.exe

SOURCES := $(wildcard $(SRC)/*.cpp)
OBJECTS := $(patsubst $(SRC)/%.cpp,$(OBJ)/%.o,$(SOURCES))
DEPENDS := $(OBJECTS:.o=.d)

.PHONY: debug clean-debug clean

debug: $(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(OBJECTS)
	$(CXX) $^ -o $@ $(LIBRARIES)

$(OBJ)/%.o : $(SRC)/%.cpp
	$(CXX) $(CXX_FLAGS) -I$(INC) -c -o $@ $<

clean:
	-rm $(BIN)/$(EXECUTABLE) $(OBJ)/*

-include $(DEPENDS)