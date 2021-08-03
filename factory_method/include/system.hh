#include "on_lattice.hh"
#include "cluster.hh"
#include "particle.hh"

#ifndef DLMA_SYSTEM_H
#define DLMA_SYSTEM_H

namespace simulation{

class dlma_system{

    public:

        dlma_system

        void check_aggregation() {};
        void initialize_system(char *filename) {};
        void move_constituent(const int i) {};

    private:

        vector<constituent*> all_particles;
        vector<constituent*> aggregates;


};



}

#endif