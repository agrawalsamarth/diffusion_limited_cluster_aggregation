IDIR =./include
CXX=g++
CPPFLAGS=-I$(IDIR) -std=c++11 -O3
LDFLAGS=-I$(IDIR) -std=c++11 -O3

SRC_DIR = ./src
OBJ_DIR = ./obj
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
BIN_SRC_DIR = ./bin_src
BIN_DIR = ./bin

all: simulation validation

simulation: $(OBJ_FILES) | $(BIN_DIR)
	$(CXX) $(CPPFLAGS) -c -o simulation.o target/simulation.cpp
	$(CXX) $(LDFLAGS) -o $@ $^ simulation.o
	mv simulation ./bin/

validation: $(OBJ_FILES) | $(BIN_DIR)
	$(CXX) $(CPPFLAGS) -c -o validation.o target/validation.cpp
	$(CXX) $(LDFLAGS) -o $@ $^ validation.o
	mv validation ./bin/

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CPPFLAGS) -c -o $@ $<

$(BIN_DIR):
	mkdir $(BIN_DIR)

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

clean:
	rm *.o
	rm ./bin/*