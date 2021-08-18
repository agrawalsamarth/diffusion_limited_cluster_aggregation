#include <check_aggregation.hh>

namespace simulation{

check_aggregation::check_aggregation(dlma_system *system_state, normal_bind *bind_system){

    sys_state = system_state;
    bind_sys  = bind_system;

}

void check_aggregation::check_for_aggregation(constituent<int> *c_1){

    bool is_checked = false;
    int  particle_id;

    cluster_id = c_1->get_id();

    for (int i = 0; i < c_1->get_size(); i++){

        neighbours  = c_1->get_neighbour_list(i);
        particle_id = c_1->get_element_id(i); 

        for (int j = 0; j < neighbours.size(); j++) { 

            neighbour_id = neighbours[j];

            if (neighbour_id != -1){

                neighbour_cluster_id = sys_state->get_id_map(neighbour_id);
                temp = sys_state->get_aggregate(neighbour_cluster_id);

                if (!temp){
                    std::cout<<"NULL pointer encountered"<<std::endl;
                    exit(EXIT_FAILURE);
                }


                if (neighbour_cluster_id != cluster_id){
                    sys_state->add_attachment(c_1);
                    check_for_aggregation(bind_sys->bind_aggregates(c_1, temp));
                    is_checked = true;
                }


            }

            if (is_checked)
                break;

            
        }

        if (is_checked)
            break;

    }


} 


}