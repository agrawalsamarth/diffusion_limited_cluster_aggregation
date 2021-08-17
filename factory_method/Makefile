IDIR =./include
CXX=g++
CPPFLAGS=-g -I$(IDIR) -std=c++11 -O3
LDFLAGS=-I$(IDIR) -std=c++11 -O3

SRC_DIR = ./src
OBJ_DIR = ./obj
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
BIN_SRC_DIR = ./bin_src
BIN_DIR = ./bin

all: test

test: $(OBJ_FILES) | $(BIN_DIR)
	$(CXX)  $(CPPFLAGS) -c -o test.o target/test.cpp
	$(CXX)  $(LDFLAGS) -o $(BIN_DIR)/$@ $^ test.o

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX)  $(CPPFLAGS) -c -o $@ $<

$(BIN_DIR):
	mkdir $(BIN_DIR)

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

clean:
	rm *.o
	rm -rf $(BIN_DIR)
	rm -rf $(OBJ_DIR)
