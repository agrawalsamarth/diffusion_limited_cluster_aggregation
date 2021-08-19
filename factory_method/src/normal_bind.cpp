#include <normal_bind.hh>

namespace simulation{

normal_bind::normal_bind(dlma_system *system_ptr){

    sys_state = system_ptr;

}

constituent<int>* normal_bind::bind_aggregates(constituent<int> *c_1, constituent<int> *c_2){

    //std::cout<<"1"<<std::endl;

    std::string name_type = "cluster";
    constituent<int> *temp;

    temp = factory.create_constituent(sys_state->get_latest_cluster_id(), sys_state->get_lattice(), sys_state->get_dim(), name_type, sys_state->get_box());

    for (int i = 0; i < c_1->get_size(); i++){
        temp->add_constituent(c_1->get_element(i));
    }

    for (int i = 0; i < c_2->get_size(); i++){
        temp->add_constituent(c_2->get_element(i));
    }

    temp->calculate_aggregate_mass();
    temp->set_current_seed_status(1);

    sys_state->remove_aggregate(c_1->get_id());
    sys_state->remove_aggregate(c_2->get_id());
    sys_state->add_aggregate(temp);
    sys_state->build_id_map();

    return temp;

}


}