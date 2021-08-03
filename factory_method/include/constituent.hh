#include "on_lattice.hh"
#include <vector>


#ifndef CONSTITUENT_H
#define CONSTITUENT_H

namespace simulation{

template <typename type>
class constituent{

    public:

        virtual void   set_mass(const double constituent_mass) {}; 
        virtual double get_mass() {};

        virtual void  move(type *delta_x) {};
        virtual type  pos(const int axis) const {};
        virtual type& pos(const int axis) {};
        virtual type  element_pos(const int i, const int axis) const {};
        virtual type& element_pos(const int i, const int axis) {};

        virtual void add_constituent(constituent *single_element) {};

        virtual void add_constituent_to_cell() {};
        virtual void remove_constituent_from_cell() {};

};



}

#endif