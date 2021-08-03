#include "constituent.hh"

#ifndef PARTICLE_H
#define PARTICLE_H

namespace simulation{

template <typename type>
class particle: public constituent<type>{

    public:

        particle(const int dim, simulation_box *system_box);

        void   move(type *delta_x);
        double get_mass();
        void   set_mass(const double constituent_mass);

        type  pos(const int axis) const; 
        type& pos(const int axis);

        void add_constituent_to_cell();
        void remove_constituent_from_cell();
        
    private:

        int             D;
        int             id;
        double          mass_;
        simulation_box *box;
        type           *pos_;
        


};


}

#endif