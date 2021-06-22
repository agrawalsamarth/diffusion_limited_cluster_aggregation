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

all: unfold radius_of_gyration sq_vs_q pair_correlation

unfold: $(OBJ_FILES) | $(BIN_DIR)
	$(CXX) $(CPPFLAGS) -c -o unfold.o target/unfold.cpp
	$(CXX) $(LDFLAGS) -o $(BIN_DIR)/$@ $^ unfold.o

radius_of_gyration: $(OBJ_FILES) | $(BIN_DIR)
	$(CXX) $(CPPFLAGS) -c -o radius_of_gyration.o target/radius_of_gyration.cpp
	$(CXX) $(LDFLAGS)  -o $(BIN_DIR)/$@ $^ radius_of_gyration.o

sq_vs_q: $(OBJ_FILES) | $(BIN_DIR)
	$(CXX) $(CPPFLAGS) -c -o sq_vs_q.o target/sq_vs_q.cpp
	$(CXX) $(LDFLAGS) -o $(BIN_DIR)/$@ $^ sq_vs_q.o

pair_correlation: $(OBJ_FILES) | $(BIN_DIR)
	$(CXX) $(CPPFLAGS) -c -o pair_correlation.o target/pair_correlation.cpp
	$(CXX) $(LDFLAGS) -o $(BIN_DIR)/$@ $^ pair_correlation.o

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CPPFLAGS) -c -o $@ $<

$(BIN_DIR):
	mkdir $(BIN_DIR)

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

clean:
	rm *.o
	rm -rf $(BIN_DIR)
	rm -rf $(OBJ_DIR)