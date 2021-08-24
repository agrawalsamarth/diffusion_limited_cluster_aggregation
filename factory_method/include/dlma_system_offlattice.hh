#include <dlma_system.hh>

#ifndef DLMA_SYSTEM_OFFLATTICE_H
#define DLMA_SYSTEM_OFFLATTICE_H

namespace simulation{

template <typename type>
class dlma_system_offlattice: public dlma_system<type>{

    public:

        dlma_system_offlattice(char *params_name);
        void initialize_system();
        void move_aggregate(int i, type *dr);
    
    private:

        void fix_overlap(const int i, type *dr);
        constituent<type> *temp;
        type *delta_r;
        std::vector<int> neighbours;

};



}

#endif