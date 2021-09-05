#include "simulation_box.hh"

#ifndef ON_LATTICE_H
#define ON_LATTICE_H

namespace simulation{

template <typename type>
class on_lattice: public simulation_box<type>{

    public:

        on_lattice(const int dim, type *box_lengths, std::vector<boundary_conditions<type>*> system_bc);
        ~on_lattice();

        type  get_refill(type x, int axis);
        type  get_L(const int axis);

        void add_particle_to_cell(const int id, type *pos);
        void remove_particle_from_cell(const int id, type *pos);
        int  get_particle_id(type *pos);

        std::vector<int> get_neighbour_list(type *pos);

        int get_periodicity(const int axis);
        
    private:

        std::vector<boundary_conditions<type>*> box_bc;
        type *L;
        int *L_eff;
        int *grid;
        int  counter;
        int  D;
        std::vector<int> neighbours;
        type *temp_pos;
        type *neighbour_pos;
        int *periodic;


};


}

#endif