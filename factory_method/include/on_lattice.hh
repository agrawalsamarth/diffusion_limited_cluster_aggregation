#include "simulation_box.hh"

#ifndef ON_LATTICE_H
#define ON_LATTICE_H

namespace simulation{

class on_lattice: public simulation_box{

    public:

        on_lattice(const int dim, int *box_lengths, std::vector<boundary_conditions*> system_bc);

        int get_refill(int x, int axis);

        /*void add_bc(boundary_conditions* bc);
        void set_L(const int L_val, const int axis);
        int  get_L(const int axis);*/

        void add_particle_to_cell(const int id, int *pos);
        void remove_particle_from_cell(const int id, int *pos);
        int  get_particle_id(int *pos);

        std::vector<int> get_neighbour_list(int *pos);
        
    private:

        std::vector<boundary_conditions*> box_bc;
        int *L;
        int *L_eff;
        int *grid;
        int  counter;
        int  D;
        std::vector<int> neighbours;
        int *temp_pos;
        int *neighbour_pos;


};


}

#endif