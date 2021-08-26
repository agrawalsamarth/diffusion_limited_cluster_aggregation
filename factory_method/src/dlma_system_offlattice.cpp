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

    name_type = "particle";
    image = this->factory.create_constituent(this->N, this->lattice, this->D, name_type, this->box);

    this->calculate_propensity();
    
}

template <typename type>
void dlma_system_offlattice<type>::move_aggregate(int i, type *dr)
{

    type scale = fix_overlap(i, dr);

    for (int axis = 0; axis < this->D; axis++)
        dr[axis] = scale * dr[axis];

    this->aggregates[i]->remove_constituent_from_cell();
    this->aggregates[i]->move(dr);
    this->aggregates[i]->add_constituent_to_cell();


}

template<typename type>
type dlma_system_offlattice<type>::fix_overlap(int i, type *dr)
{

    constituent<type> *c_1 = this->aggregates[i];
    constituent<type> *ref_particle;
    constituent<type> *nb_particle;
    //constituent<type> *image;
    type distance;
    type fraction = 1.;

    int particle_id;
    int cluster_id = c_1->get_id();
    int neighbour_id;
    int neighbour_cluster_id;

    type diff[this->D];
    type beta;
    type q;
    type dia_distance;
    type diff_2;
    type alpha;

    for (int i = 0; i < c_1->get_size(); i++){

        ref_particle = c_1->get_element(i);

        for (int axis = 0; axis < this->D; axis++)
            image->pos(axis) = ref_particle->pos(axis);

        image->move(dr);

        actual_list = ref_particle->get_neighbour_list();
        image_list  = image->get_neighbour_list();
        
        neighbours.clear();
        neighbours.reserve(actual_list.size() + image_list.size());
        neighbours.insert( neighbours.end(), actual_list.begin(), actual_list.end() );
        neighbours.insert( neighbours.end(), image_list.begin(), image_list.end() );
           
        for (int j = 0; j < neighbours.size(); j++) { 

            neighbour_id = neighbours[j];
            neighbour_cluster_id = this->get_id_map(neighbour_id);

            if (neighbour_cluster_id != cluster_id){

                nb_particle = this->get_particle_by_id(neighbour_id);

                for (int axis = 0; axis < this->D; axis++)
                    diff[axis] = this->box->get_periodic_distance(nb_particle->pos(axis), ref_particle->pos(axis), axis);

                q = 0;

                for (int axis = 0; axis < this->D; axis++)
                    q += diff[axis] * dr[axis];

                diff_2 = 0;

                for (int axis = 0; axis < this->D; axis++)
                    diff_2 += diff[axis] * diff[axis];

                dia_distance = 0.5 * (ref_particle->get_diameter() + nb_particle->get_diameter());
                dia_distance = dia_distance * dia_distance;

                beta = (q * q) - diff_2 + dia_distance;

                //if (fabs(diff_2 - dia_distance) < 1e-10)
                    //return 0;

                if (beta >= 0.){

                    alpha = q - sqrt(beta);

                    if (alpha >= 0){

                        if (alpha < fraction)
                            fraction = alpha;

                    }

                }


            }
        }

    }

    return fraction;


}

template<typename type>
void dlma_system_offlattice<type>::add_attachment(const int i, const int j)
{
    this->attachments[i].push_back(j);
    this->attachments[j].push_back(i);
}

template class dlma_system_offlattice<int>;
template class dlma_system_offlattice<double>;

}