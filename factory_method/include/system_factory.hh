#include "particle.hh"
#include "cluster.hh"
#include "periodic_bc.hh"
#include "on_lattice.hh"

#ifndef SYSTEM_FACTORY_H
#define SYSTEM_FACTORY_H

namespace simulation{

class system_factory{

    public:

        constituent*         create_constituent(int lattice, int dim, std::string name_type, simulation_box *box);
        simulation_box*      create_simulation_box(int lattice, int dim, int *box_lengths, vector<boundary_conditions*> system_bc);
        //simulation_box*      create_simulation_box(int lattice, int dim, double *box_lengths);
        boundary_conditions* create_boundary_conditions(std::string name_type);


};

}


#endif