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

all: unfold radius_of_gyration sq_vs_q pair_correlation psd_test

density_correlation: $(OBJ_FILES) | $(BIN_DIR)
	$(CXX) $(CPPFLAGS) -c -o density_correlation.o target/density_correlation.cpp -L /home/samarth/codes/voronoi/lib -lvoro++
	$(CXX) $(LDFLAGS) -o $(BIN_DIR)/$@ $^ density_correlation.o -L /home/samarth/codes/voronoi/lib -lvoro++

unfold: $(OBJ_FILES) | $(BIN_DIR)
	$(CXX) $(CPPFLAGS) -c -o unfold.o target/unfold.cpp -L /home/samarth/codes/voronoi/lib -lvoro++
	$(CXX) $(LDFLAGS) -o $(BIN_DIR)/$@ $^ unfold.o -L /home/samarth/codes/voronoi/lib -lvoro++

radius_of_gyration: $(OBJ_FILES) | $(BIN_DIR)
	$(CXX) $(CPPFLAGS) -c -o radius_of_gyration.o target/radius_of_gyration.cpp -L /home/samarth/codes/voronoi/lib -lvoro++
	$(CXX) $(LDFLAGS)  -o $(BIN_DIR)/$@ $^ radius_of_gyration.o -L /home/samarth/codes/voronoi/lib -lvoro++

sq_vs_q: $(OBJ_FILES) | $(BIN_DIR)
	$(CXX) $(CPPFLAGS) -c -o sq_vs_q.o target/sq_vs_q.cpp -L /home/samarth/codes/voronoi/lib -lvoro++
	$(CXX) $(LDFLAGS) -o $(BIN_DIR)/$@ $^ sq_vs_q.o -L /home/samarth/codes/voronoi/lib -lvoro++

pair_correlation: $(OBJ_FILES) | $(BIN_DIR)
	$(CXX) $(CPPFLAGS) -c -o pair_correlation.o target/pair_correlation.cpp -L /home/samarth/codes/voronoi/lib -lvoro++
	$(CXX) $(LDFLAGS) -o $(BIN_DIR)/$@ $^ pair_correlation.o -L /home/samarth/codes/voronoi/lib -lvoro++

psd_test: $(OBJ_FILES) | $(BIN_DIR)
	$(CXX) $(CPPFLAGS) -c -o psd_test.o target/psd_test.cpp -L /home/samarth/codes/voronoi/lib -lvoro++
	$(CXX) $(LDFLAGS) -o $(BIN_DIR)/$@ $^ psd_test.o -L /home/samarth/codes/voronoi/lib -lvoro++



$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CPPFLAGS) -c -o $@ $< -L /home/samarth/codes/voronoi/lib -lvoro++

$(BIN_DIR):
	mkdir $(BIN_DIR)

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

clean:
	rm *.o
	rm -rf $(BIN_DIR)
	rm -rf $(OBJ_DIR)
