#include <dlma_system.hh>

#ifndef DLMA_SYSTEM_ONLATTICE_H
#define DLMA_SYSTEM_ONLATTICE_H

namespace simulation{

template <typename type>
class dlma_system_onlattice: public dlma_system<type>{

    public:

        dlma_system_onlattice(char *params_name);
        void initialize_system();
        bool check_viability(constituent<type> *c_1, type *dr);
        void move_aggregate(int i, type *dr);
        void print_grid();

    private:

        void initialize_system_for_percolation();

};



}

#endif