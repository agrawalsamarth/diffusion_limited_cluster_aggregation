#include "check_aggregation.hh"
#include "brownian_movement.hh"
#include "mass_aggregation_condition.hh"
#include "dlma_save_config.hh"
#include "normal_bind.hh"

#ifndef ITERATOR_FACTORY_H
#define ITERATOR_FACTORY_H

namespace simulation{

class iterator_factory{

    public:

        particle_movement* create_movement(std::string name_type, int dim, int rng_seed);
        aggregation_condition* create_aggregation_condition(std::string name_type, dlma_system *system_state); 
        check_aggregation* create_check_aggregation(std::string name_type, dlma_system *system_state, normal_bind *bind_system, aggregation_condition *ref_condition);
        save_config* create_save_config(std::string name_type, dlma_system *ref_sys, simulation_box *ref_box);
        normal_bind* create_bind_system(std::string name_type, dlma_system *system_ptr);



};



}


#endif