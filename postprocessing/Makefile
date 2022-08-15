IDIR =./include
CXX=g++
#CPPFLAGS=-g -I$(IDIR) -I/home/samarth/codes/voronoi/include -std=c++11 -O3 -w -Wall
CPPFLAGS=-g -I$(IDIR) -O3
LDFLAGS=-O3
POSTFIX=-L/home/samarth/codes/voronoi/lib/ -lvoro++


SRC_DIR = ./src
OBJ_DIR = ./obj
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
BIN_SRC_DIR = ./bin_src
BIN_DIR = ./bin

all: unfold radius_of_gyration sq_vs_q pair_correlation percolation load_bearing_paths lr_iq lbp_brute_force percolation_invA
#all: psd_test

density_correlation: $(OBJ_FILES) | $(BIN_DIR)
	$(CXX) $(CPPFLAGS) -c -o density_correlation.o target/density_correlation.cpp 	
	$(CXX) $(LDFLAGS) -o $(BIN_DIR)/$@ $^ density_correlation.o

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

psd_test: $(OBJ_FILES) | $(BIN_DIR)
	$(CXX) $(CPPFLAGS) -c -o psd_test.o target/psd_test.cpp
	$(CXX) $(LDFLAGS) -o $(BIN_DIR)/$@ $^ psd_test.o

percolation: $(OBJ_FILES) | $(BIN_DIR)
	$(CXX) $(CPPFLAGS) -c -o percolation.o target/percolation.cpp
	$(CXX) $(LDFLAGS) -o $(BIN_DIR)/$@ $^ percolation.o

load_bearing_paths: $(OBJ_FILES) | $(BIN_DIR)
	$(CXX) $(CPPFLAGS) -c -o load_bearing_paths.o target/load_bearing_paths.cpp
	$(CXX) $(LDFLAGS) -o $(BIN_DIR)/$@ $^ load_bearing_paths.o

lbp_brute_force: $(OBJ_FILES) | $(BIN_DIR)
	$(CXX) $(CPPFLAGS) -c -o lbp_brute_force.o target/lbp_brute_force.cpp
	$(CXX) $(LDFLAGS) -o $(BIN_DIR)/$@ $^ lbp_brute_force.o

percolation_invA: $(OBJ_FILES) | $(BIN_DIR)
	$(CXX) $(CPPFLAGS) -c -o percolation_invA.o target/percolation_invA.cpp
	$(CXX) $(LDFLAGS) -o $(BIN_DIR)/$@ $^ percolation_invA.o

lb_bonds_invA: $(OBJ_FILES) | $(BIN_DIR)
	$(CXX) $(CPPFLAGS) -c -o lb_bonds_invA.o target/lb_bonds_invA.cpp
	$(CXX) $(LDFLAGS) -o $(BIN_DIR)/$@ $^ lb_bonds_invA.o
	
lb_bonds_clusterwise_invA: $(OBJ_FILES) | $(BIN_DIR)
	$(CXX) $(CPPFLAGS) -c -o lb_bonds_clusterwise_invA.o target/lb_bonds_clusterwise_invA.cpp
	$(CXX) $(LDFLAGS) -o $(BIN_DIR)/$@ $^ lb_bonds_clusterwise_invA.o

lr_iq: $(OBJ_FILES) | $(BIN_DIR)
	$(CXX) $(CPPFLAGS) -c -o lr_iq.o target/lr_iq.cpp
	$(CXX) $(LDFLAGS) -o $(BIN_DIR)/$@ $^ lr_iq.o






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
