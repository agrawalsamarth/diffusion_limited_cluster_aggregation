#include <dlma_system_onlattice.hh>

namespace simulation{

template <typename type>
dlma_system_onlattice<type>::dlma_system_onlattice(char *params_name)
{
    this->read_params_parser(params_name);

    if (this->system_type == "dlma")
        initialize_system();

    else if (this->system_type == "random_site_percolation")
        initialize_system_for_percolation();

    else{
        std::cout<<"system not defined"<<std::endl;
        exit(EXIT_FAILURE);
    }

}

template <typename type>
void dlma_system_onlattice<type>::initialize_system()
{

    bool is_placed;
    constituent<type> *temp;

    std::string name_type = "particle";

    type temp_pos[this->D];

    for (int i = 0; i < this->N; i++){

        temp = this->factory.create_constituent(i, this->lattice, this->D, name_type, this->box);

        is_placed = false;

        temp->set_diameter(1.);

        if (i < this->N_s){
            temp->set_mass(this->seed_mass);
            temp->set_original_seed_status(1);
            temp->set_current_seed_status(1);
        }

        else{
            temp->set_mass(1.);
            temp->set_original_seed_status(0);
            temp->set_current_seed_status(0);
        }

        while (is_placed == false){

            for (int axis = 0; axis < this->D; axis++)
                temp_pos[axis] = (int)(this->dis(this->generator) * this-> L[axis]);

            if (this->box->get_particle_id(temp_pos) == -1){

                for (int axis = 0; axis < this->D; axis++)
                    temp->pos(axis) = temp_pos[axis];

                temp->add_constituent_to_cell();
                this->all_particles.push_back(temp);
                is_placed = true;
            }

        }

    }



    name_type = "cluster";

    for (int i = 0; i < this->N; i++){

        temp = this->factory.create_constituent(this->get_latest_cluster_id(), this->lattice, this->D, name_type, this->box);
        temp->add_constituent(this->all_particles[i]);
        temp->calculate_aggregate_mass();
        //std::cout<<"seed mass = "<<get_seedmass()<<std::endl;

        this->aggregates.push_back(temp);

    }

    this->build_id_map();
    this->calculate_propensity();
    
}

template<typename type>
void dlma_system_onlattice<type>::initialize_system_for_percolation()
{

    constituent<type> *temp;
    std::string name_type = "particle";

    int L_eff[this->D];

    for (int axis = 0; axis < this->D; axis++)
        L_eff[axis] = 1;

    for (int axis = 0; axis < this->D; axis++)
    {
        for (int itr = axis+1; itr < this->D; itr++)
            L_eff[axis] *= this->L[itr];
    }

    //for (int axis = 0; axis < this->D; axis++)
        //std::cout<<"L_eff = "<<L[axis]<<std::endl;

    int L_total = 1;

    for (int axis = 0; axis < this->D; axis++)
        L_total *= this->L[axis];

    int div, rem;

    this->N = 0;

    for (int i = 0; i < L_total; i++){

        if (this->dis(this->generator) < this->phi){

            temp = this->factory.create_constituent(this->N, this->lattice, this->D, name_type, this->box);
            this->N += 1;
            temp->set_diameter(1.);
            temp->set_mass(this->seed_mass);
            temp->set_original_seed_status(1);
            temp->set_current_seed_status(1);

            rem = i;

            for (int axis = 0; axis < this->D; axis++){
                div = rem/L_eff[axis];
                temp->pos(axis) = div;
                rem = rem % L_eff[axis];
            }

            //std::cout<<temp->pos(0)<<" "<<temp->pos(1)<<std::endl;

            temp->add_constituent_to_cell();
            this->all_particles.push_back(temp);

        }

    }

    name_type = "cluster";

    for (int i = 0; i < this->N; i++){

        temp = this->factory.create_constituent(this->get_latest_cluster_id(), this->lattice, this->D, name_type, this->box);
        temp->add_constituent(this->all_particles[i]);
        temp->calculate_aggregate_mass();
        //std::cout<<"seed mass = "<<get_seedmass()<<std::endl;

        this->aggregates.push_back(temp);

    }

    this->attachments.resize(this->N);
    this->build_id_map();

    for (int i = 0; i < this->N; i++)
        this->add_attachment(this->aggregates[i]);
    

}



template <typename type>
bool dlma_system_onlattice<type>::check_viability(constituent<type> *c_1, type *dr)
{
    type temp_pos[this->D];

    for (int axis = 0; axis < this->D; axis++)
        temp_pos[axis] = 0;

    int cluster_id = c_1->get_id();
    int neighbour_cluster_id;
    int neighbour_id;

    constituent<type> *temp;


    this->is_viable = true;

    for (int i = 0; i < c_1->get_size(); i++){

        temp = c_1->get_element(i);

        for (int axis = 0; axis < this->D; axis++){
            temp_pos[axis] = this->box->get_refill(temp->pos(axis)+dr[axis], axis);
        }

        neighbour_id = this->box->get_particle_id(temp_pos);

        if (neighbour_id != -1){
            neighbour_cluster_id = this->get_id_map(neighbour_id);

            if (neighbour_cluster_id != cluster_id)
                this->is_viable=false;
        }

    }

    return this->is_viable;

}

template <typename type>
void dlma_system_onlattice<type>::move_aggregate(int i, type *dr)
{

    if (check_viability(this->aggregates[i], dr)){
        this->aggregates[i]->remove_constituent_from_cell();
        this->aggregates[i]->move(dr);
        this->aggregates[i]->add_constituent_to_cell();
    }


}

template <typename type>
void dlma_system_onlattice<type>::print_grid()
{
    type temp_pos[this->D];
    int temp_id;

    for (int i = 0; i < this->L[0]; i++){
        for (int j = 0; j < this->L[1]; j++){

            temp_pos[0] = i;
            temp_pos[1] = j;

            temp_id = this->box->get_particle_id(temp_pos);

            std::cout<<temp_id<<"\t";

        }
        std::cout<<"\n"<<std::endl;
    }

}

template class dlma_system_onlattice<int>;
template class dlma_system_onlattice<double>;

}