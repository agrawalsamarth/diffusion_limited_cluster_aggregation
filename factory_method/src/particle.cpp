#include <particle.hh>

namespace simulation{

template<typename type>
particle<type>::particle(const int dim, simulation_box *system_box){
    
    D   = dim;
    box = system_box;

    pos_ = (type*)malloc(sizeof(type)*D);

    for (int axis = 0; axis < D; axis++)
        pos_[axis] = 0;

    //for (int axis = 0; axis < D; axis++)
        //std::cout<<"L="<<box->get_L(axis)<<std::endl;

}

template<typename type>
void particle<type>::set_mass(const double constituent_mass)
{ mass_ = constituent_mass;}

template<typename type>
double particle<type>::get_mass()
{ return mass_;}

template<typename type>
type particle<type>::pos(const int axis) const
{ return pos_[axis];}

template<typename type>
type& particle<type>::pos(const int axis)
{ return pos_[axis];}

template<typename type>
void particle<type>::move(type *delta_x)
{

    for (int axis = 0; axis < D; axis++)
        pos_[axis] = box->get_refill(pos_[axis]+delta_x[axis], axis);
        //pos_[axis] = pos_[axis]+delta_x[axis];

}

template<typename type>
void particle<type>::add_constituent_to_cell()
{ box->add_particle_to_cell(id, pos_); }

template<typename type>
void particle<type>::remove_constituent_from_cell()
{ box->remove_particle_from_cell(id, pos_); }


template class particle<int>;
template class particle<double>;

}