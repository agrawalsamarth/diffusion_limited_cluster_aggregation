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
	$(CXX) $(LDFLAGS) -o $@ $^ unfold.o
	mv unfold ./bin/

radius_of_gyration: $(OBJ_FILES) | $(BIN_DIR)
	$(CXX) $(CPPFLAGS) -c -o radius_of_gyration.o target/radius_of_gyration.cpp
	$(CXX) $(LDFLAGS) -o $@ $^ radius_of_gyration.o
	mv radius_of_gyration ./bin/

sq_vs_q: $(OBJ_FILES) | $(BIN_DIR)
	$(CXX) $(CPPFLAGS) -c -o sq_vs_q.o target/sq_vs_q.cpp
	$(CXX) $(LDFLAGS) -o $@ $^ sq_vs_q.o
	mv sq_vs_q ./bin/

pair_correlation: $(OBJ_FILES) | $(BIN_DIR)
	$(CXX) $(CPPFLAGS) -c -o pair_correlation.o target/pair_correlation.cpp
	$(CXX) $(LDFLAGS) -o $@ $^ pair_correlation.o
	mv pair_correlation ./bin/

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CPPFLAGS) -c -o $@ $<

$(BIN_DIR):
	mkdir $(BIN_DIR)

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

clean:
	rm *.o
	rm ./bin/*