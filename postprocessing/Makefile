IDIR =./include
CXX=g++
#CPPFLAGS=-g -I$(IDIR) -I/home/samarth/codes/voronoi/include -std=c++11 -O3 -w -Wall
#CPPFLAGS=-g -I$(IDIR) -O3
CPPFLAGS=-I$(IDIR) -O3
#LDFLAGS=-O3
POSTFIX=-L/home/samarth/codes/voronoi/lib/ -lvoro++
VOROFLAG=./include/voro++/lib/libvoro++.a

SRC_DIR = ./src
OBJ_DIR = ./obj
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
BIN_SRC_DIR = ./bin_src
BIN_DIR = ./bin

all: unfold radius_of_gyration sq_vs_q pair_correlation percolation load_bearing_paths lr_iq lbp_brute_force percolation_invA chemical_distance lb_bonds_clusterwise_invA lbp_bfs long_range_scattering eq_energy create_dangling_free_config
#all: psd_test

density_correlation: $(OBJ_FILES) | $(BIN_DIR)
	$(CXX) $(CPPFLAGS) -c -o density_correlation.o target/density_correlation.cpp 	
	$(CXX) $(LDFLAGS) -o $(BIN_DIR)/$@ $^ $(VOROFLAG) density_correlation.o

unfold: $(OBJ_FILES) | $(BIN_DIR)
	$(CXX) $(CPPFLAGS) -c -o unfold.o target/unfold.cpp
	$(CXX) $(LDFLAGS) -o $(BIN_DIR)/$@ $^ $(VOROFLAG) unfold.o

radius_of_gyration: $(OBJ_FILES) | $(BIN_DIR)
	$(CXX) $(CPPFLAGS) -c -o radius_of_gyration.o target/radius_of_gyration.cpp
	$(CXX) $(LDFLAGS)  -o $(BIN_DIR)/$@ $^ $(VOROFLAG) radius_of_gyration.o

sq_vs_q: $(OBJ_FILES) | $(BIN_DIR)
	$(CXX) $(CPPFLAGS) -c -o sq_vs_q.o target/sq_vs_q.cpp
	$(CXX) $(LDFLAGS) -o $(BIN_DIR)/$@ $^ $(VOROFLAG) sq_vs_q.o

pair_correlation: $(OBJ_FILES) | $(BIN_DIR)
	$(CXX) $(CPPFLAGS) -c -o pair_correlation.o target/pair_correlation.cpp
	$(CXX) $(LDFLAGS) -o $(BIN_DIR)/$@ $^ $(VOROFLAG) pair_correlation.o

psd_test: $(OBJ_FILES) | $(BIN_DIR)
	$(CXX) $(CPPFLAGS) -c -o psd_test.o target/psd_test.cpp
	$(CXX) $(LDFLAGS) -o $(BIN_DIR)/$@ $^ $(VOROFLAG) psd_test.o

percolation: $(OBJ_FILES) | $(BIN_DIR)
	$(CXX) $(CPPFLAGS) -c -o percolation.o target/percolation.cpp
	$(CXX) $(LDFLAGS) -o $(BIN_DIR)/$@ $^ $(VOROFLAG) percolation.o

load_bearing_paths: $(OBJ_FILES) | $(BIN_DIR)
	$(CXX) $(CPPFLAGS) -c -o load_bearing_paths.o target/load_bearing_paths.cpp
	$(CXX) $(LDFLAGS) -o $(BIN_DIR)/$@ $^ $(VOROFLAG) load_bearing_paths.o

lbp_brute_force: $(OBJ_FILES) | $(BIN_DIR)
	$(CXX) $(CPPFLAGS) -c -o lbp_brute_force.o target/lbp_brute_force.cpp
	$(CXX) $(LDFLAGS) -o $(BIN_DIR)/$@ $^ $(VOROFLAG) lbp_brute_force.o

percolation_invA: $(OBJ_FILES) | $(BIN_DIR)
	$(CXX) $(CPPFLAGS) -c -o percolation_invA.o target/percolation_invA.cpp
	$(CXX) $(LDFLAGS) -o $(BIN_DIR)/$@ $^ $(VOROFLAG) percolation_invA.o

lb_bonds_invA: $(OBJ_FILES) | $(BIN_DIR)
	$(CXX) $(CPPFLAGS) -c -o lb_bonds_invA.o target/lb_bonds_invA.cpp
	$(CXX) $(LDFLAGS) -o $(BIN_DIR)/$@ $^ $(VOROFLAG) lb_bonds_invA.o
	
lb_bonds_clusterwise_invA: $(OBJ_FILES) | $(BIN_DIR)
	$(CXX) $(CPPFLAGS) -c -o lb_bonds_clusterwise_invA.o target/lb_bonds_clusterwise_invA.cpp
	$(CXX) $(LDFLAGS) -o $(BIN_DIR)/$@ $^ $(VOROFLAG) lb_bonds_clusterwise_invA.o
	
lb_bonds_via_cg: $(OBJ_FILES) | $(BIN_DIR)
	$(CXX) $(CPPFLAGS) -c -o lb_bonds_via_cg.o target/lb_bonds_via_cg.cpp
	$(CXX) $(LDFLAGS) -o $(BIN_DIR)/$@ $^ $(VOROFLAG) lb_bonds_via_cg.o
	
chemical_distance: $(OBJ_FILES) | $(BIN_DIR)
	$(CXX) $(CPPFLAGS) -c -o chemical_distance.o target/chemical_distance.cpp
	$(CXX) $(LDFLAGS) -o $(BIN_DIR)/$@ $^ $(VOROFLAG) chemical_distance.o
	
lr_iq: $(OBJ_FILES) | $(BIN_DIR)
	$(CXX) $(CPPFLAGS) -c -o lr_iq.o target/lr_iq.cpp
	$(CXX) $(LDFLAGS) -o $(BIN_DIR)/$@ $^ $(VOROFLAG) lr_iq.o
	
lbp_bfs: $(OBJ_FILES) | $(BIN_DIR)
	$(CXX) $(CPPFLAGS) -c -o lbp_bfs.o target/lbp_bfs.cpp
	$(CXX) $(LDFLAGS) -o $(BIN_DIR)/$@ $^ $(VOROFLAG) lbp_bfs.o
	
long_range_scattering: $(OBJ_FILES) | $(BIN_DIR)
	$(CXX) $(CPPFLAGS) -c -o long_range_scattering.o target/long_range_scattering.cpp
	$(CXX) $(LDFLAGS) -o $(BIN_DIR)/$@ $^ $(VOROFLAG) long_range_scattering.o
	
eq_energy: $(OBJ_FILES) | $(BIN_DIR)
	$(CXX) $(CPPFLAGS) -c -o eq_energy.o target/eq_energy.cpp
	$(CXX) $(LDFLAGS) -o $(BIN_DIR)/$@ $^ $(VOROFLAG) eq_energy.o	

create_dangling_free_config: $(OBJ_FILES) | $(BIN_DIR)
	$(CXX) $(CPPFLAGS) -c -o create_dangling_free_config.o target/create_dangling_free_config.cpp
	$(CXX) $(LDFLAGS) -o $(BIN_DIR)/$@ $^ $(VOROFLAG) create_dangling_free_config.o	

voronoi_volumes: $(OBJ_FILES) | $(BIN_DIR)
	$(CXX) $(CPPFLAGS) -c -o voronoi_volumes.o target/voronoi_volumes.cpp
	$(CXX) $(LDFLAGS) -o $(BIN_DIR)/$@ $^ $(VOROFLAG) voronoi_volumes.o	


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
