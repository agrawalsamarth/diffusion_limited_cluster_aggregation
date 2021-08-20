#include <dlma_system_onlattice.hh>

namespace simulation{

template <typename type>
dlma_system_onlattice<type>::dlma_system_onlattice(char *params_name)
{
    read_params_parser(params_name);
    initialize_system();
}

template <typename type>
void dlma_system_onlattice<type>::initialize_system()
{

    bool is_placed;
    constituent<type> *temp;

    std::string name_type = "particle";

    int temp_pos[D];

    for (int i = 0; i < N; i++){

        temp = factory.create_constituent(i, lattice, D, name_type, box);

        is_placed = false;

        temp->set_diameter(1.);

        if (i < N_s){
            temp->set_mass(seed_mass);
            temp->set_original_seed_status(1);
            temp->set_current_seed_status(1);
        }

        else{
            temp->set_mass(1.);
            temp->set_original_seed_status(0);
            temp->set_current_seed_status(0);
        }

        while (is_placed == false){

            for (int axis = 0; axis < D; axis++)
                temp_pos[axis] = (int)(dis(generator) * L[axis]);

            if (box->get_particle_id(temp_pos) == -1){

                for (int axis = 0; axis < D; axis++)
                    temp->pos(axis) = temp_pos[axis];

                temp->add_constituent_to_cell();
                all_particles.push_back(temp);
                is_placed = true;
            }

        }

    }



    name_type = "cluster";

    for (int i = 0; i < N; i++){

        temp = factory.create_constituent(get_latest_cluster_id(), lattice, D, name_type, box);
        temp->add_constituent(all_particles[i]);
        temp->calculate_aggregate_mass();
        //std::cout<<"seed mass = "<<get_seedmass()<<std::endl;

        aggregates.push_back(temp);

    }

    build_id_map();
    
}

template <typename type>
bool dlma_system_onlattice<type>::check_viability(constituent<type> *c_1, type *dr)
{
    int temp_pos[D];

    for (int axis = 0; axis < D; axis++)
        temp_pos[axis] = 0;

    int cluster_id = c_1->get_id();
    int neighbour_cluster_id;
    int neighbour_id;

    constituent<int> *temp;


    is_viable = true;

    for (int i = 0; i < c_1->get_size(); i++){

        temp = c_1->get_element(i);

        for (int axis = 0; axis < D; axis++){
            temp_pos[axis] = box->get_refill(temp->pos(axis)+dr[axis], axis);
        }

        neighbour_id = box->get_particle_id(temp_pos);

        if (neighbour_id != -1){
            neighbour_cluster_id = get_id_map(neighbour_id);

            if (neighbour_cluster_id != cluster_id)
                is_viable=false;
        }

    }

    return is_viable;

}

template <typename type>
void dlma_system_onlattice<type>::move_aggregate(int i, type *dr)
{

    if (check_viability(aggregates[i], dr)){
        aggregates[i]->remove_constituent_from_cell();
        aggregates[i]->move(dr);
        aggregates[i]->add_constituent_to_cell();
    }


}

template <typename type>
void dlma_system_onlattice<type>::print_grid()
{
    int temp_pos[D];
    int temp_id;

    for (int i = 0; i < L[0]; i++){
        for (int j = 0; j < L[1]; j++){

            temp_pos[0] = i;
            temp_pos[1] = j;

            temp_id = box->get_particle_id(temp_pos);

            std::cout<<temp_id<<"\t";

        }
        std::cout<<"\n"<<std::endl;
    }

}

template class dlma_system_onlattice<int>;
template class dlma_system_onlattice<double>;

}