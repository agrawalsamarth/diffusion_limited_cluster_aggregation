#include <dlma_system_offlattice.hh>

namespace simulation{

template <typename type>
dlma_system_offlattice<type>::dlma_system_offlattice(char *params_name)
{
    this->read_params_parser(params_name);
    initialize_system();
}

template <typename type>
void dlma_system_offlattice<type>::initialize_system()
{

    bool is_placed;
    constituent<type> *temp;

    std::string name_type = "particle";

    type distance;

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
                temp->pos(axis) = (this->dis(this->generator) * this-> L[axis]);

                is_placed = true;
                
                for (int j = 0; j < i; j++){
            
                    distance = this->get_interparticle_distance(temp, this->all_particles[j]);

                    if (distance < (0.5 * (temp->get_diameter() + this->all_particles[j]->get_diameter()))){
                        is_placed = false;
                        break;
                    }

                }            

        }

        temp->add_constituent_to_cell();
        this->all_particles.push_back(temp);

    }

    name_type = "cluster";

    for (int i = 0; i < this->N; i++){

        temp = this->factory.create_constituent(this->get_latest_cluster_id(), this->lattice, this->D, name_type, this->box);
        temp->add_constituent(this->all_particles[i]);
        temp->calculate_aggregate_mass();
        this->aggregates.push_back(temp);

    }

    this->build_id_map();
    
}

template <typename type>
void dlma_system_offlattice<type>::move_aggregate(int i, type *dr)
{

    this->aggregates[i]->remove_constituent_from_cell();
    this->aggregates[i]->move(dr);
    this->aggregates[i]->add_constituent_to_cell();

    fix_overlap(i, dr);

}

template<typename type>
void dlma_system_offlattice<type>::fix_overlap(int i, type *dr)
{

    delta_r = dr;

    constituent<type> *c_1 = this->aggregates[i];
    constituent<type> *particle_1;
    constituent<type> *particle_2;
    type distance;
    type max_overlap = 0.;

    int particle_id;
    int cluster_id = c_1->get_id();
    int neighbour_id;
    int neighbour_cluster_id;

    for (int i = 0; i < c_1->get_size(); i++){

        neighbours  = c_1->get_neighbour_list(i);
        particle_id = c_1->get_element_id(i); 

        for (int j = 0; j < neighbours.size(); j++) { 

            neighbour_id = neighbours[j];
            neighbour_cluster_id = this->get_id_map(neighbour_id);

            if (neighbour_cluster_id != cluster_id){

                particle_1 = this->get_particle_by_id(particle_id);
                particle_2 = this->get_particle_by_id(neighbour_id);

                distance  = this->get_interparticle_distance(particle_1, particle_2);
                distance  = 0.5 * (particle_1->get_diameter() + particle_2->get_diameter()) - distance;

                if (distance > max_overlap)
                    max_overlap = distance; 

            }
        }

    }

    std::cout<<"max_overlap="<<max_overlap<<std::endl;

    if (max_overlap > 1e-8){

        for (int axis = 0; axis < this->D; axis++)
            dr[axis] = (-1. * dr[axis])/(1. * max_overlap);

        this->move_aggregate(i, dr); 

    }

}

template class dlma_system_offlattice<int>;
template class dlma_system_offlattice<double>;

}