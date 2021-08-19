#include <iterator_factory.hh>

namespace simulation{


particle_movement* iterator_factory::create_movement(std::string name_type, int dim, int rng_seed)
{
    if (strcmp(name_type.c_str(), "brownian"))
        return new brownian_movement(dim, rng_seed);
    else{
        std::cout<<"unknown movement type"<<std::endl;
        exit(EXIT_FAILURE);
    }
}


aggregation_condition* iterator_factory::create_aggregation_condition(std::string name_type, dlma_system *system_state)
{
    if (strcmp(name_type.c_str(), "mass"))
        return new mass_aggregation_condition(system_state);
    else{
        std::cout<<"unknown aggregate condition"<<std::endl;
        exit(EXIT_FAILURE);
    }
    
}


check_aggregation* iterator_factory::create_check_aggregation(std::string name_type, dlma_system *system_state, normal_bind *bind_system, aggregation_condition *ref_condition)
{

    if (strcmp(name_type.c_str(), "normal"))
        return new check_aggregation(system_state, bind_system, ref_condition);
    else{
        std::cout<<"unknown aggregation type"<<std::endl;
        exit(EXIT_FAILURE);
    }

}

save_config* iterator_factory::create_save_config(std::string name_type, dlma_system *ref_sys, simulation_box *ref_box)
{

    if (strcmp(name_type.c_str(), "dlma"))
        return new dlma_save_config(ref_sys, ref_box);
    else{
        std::cout<<"unknown save type"<<std::endl;
        exit(EXIT_FAILURE);
    }


}


normal_bind* iterator_factory::create_bind_system(std::string name_type, dlma_system *system_ptr)
{

    if (strcmp(name_type.c_str(), "normal"))
        return new normal_bind(system_ptr);
    else{
        std::cout<<"unknown bind type"<<std::endl;
        exit(EXIT_FAILURE);
    }



}





}