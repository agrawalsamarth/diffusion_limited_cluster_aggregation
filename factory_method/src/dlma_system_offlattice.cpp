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
    //iters++;

    this->aggregates[i]->remove_constituent_from_cell();
    this->aggregates[i]->move(dr);
    this->aggregates[i]->add_constituent_to_cell();

    fix_overlap(i, dr);
    //iters=0;

}

template<typename type>
void dlma_system_offlattice<type>::fix_overlap(int i, type *dr)
{

    constituent<type> *c_1 = this->aggregates[i];
    constituent<type> *particle_1;
    constituent<type> *particle_2;
    type distance;
    type max_overlap = -1.;

    int particle_id;
    int cluster_id = c_1->get_id();
    int neighbour_id;
    int neighbour_cluster_id;

    int p_id;
    int n_id;

    type dot_product;
    type scale;

    type sum = 0;
    type dot = 0;
    type tolerance = (-1. * 1e-4);
    type diff[this->D];
    type temp_dr[this->D];

    while (max_overlap < tolerance){

        max_overlap = 0.;

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
                    distance  = distance - 0.5 * (particle_1->get_diameter() + particle_2->get_diameter());

                    //std::cout<<"distance = "<<distance<<std::endl;

                    if (distance < max_overlap){
                        max_overlap = distance;
                        //std::cout<<"max_overlap = "<<max_overlap<<"\t id1 = "<<particle_id<<"\t id2 = "<<neighbour_id<<"\t distance = "<<distance<<std::endl;

                        //for (int axis = 0; axis < this->D; axis++)
                            //std::cout<<"axis = "<<axis<<"\t"<<particle_1->pos(axis)<<"\t"<<particle_2->pos(axis)<<std::endl;

                        p_id = particle_id;
                        n_id = neighbour_id;

                    }

                }
            }

        }

        if (max_overlap < tolerance){

            particle_1 = this->get_particle_by_id(p_id);
            particle_2 = this->get_particle_by_id(n_id);

            if (!particle_1)
                std::cout<<"particle 1 is NULL"<<std::endl;

            if (!particle_2)
                std::cout<<"particle 2 is NULL"<<std::endl;

            for (int axis = 0; axis < this->D; axis++){
                diff[axis] = this->box->get_periodic_distance(particle_1->pos(axis), particle_2->pos(axis), axis);
                //std::cout<<diff[axis]<<std::endl;
            }

            sum = 0.;

            for (int axis = 0; axis < this->D; axis++){
                sum += (diff[axis] * diff[axis]);
            }

            sum = sqrt(sum);

            for (int axis = 0; axis < this->D; axis++)
                diff[axis] /= (1. * sum);

            //for (int axis = 0; axis < this->D; axis++)
                //std::cout<<"normalized diff = "<<diff[axis]<<std::endl;

            /*dot = 0.;

            for (int axis = 0; axis < this->D; axis++)
                dot += diff[axis] * dr[axis];

            dot = fabs(dot);

            scale = (-1. * max_overlap)/(1. * dot);*/

            for (int axis = 0; axis < this->D; axis++){
                temp_dr[axis] = max_overlap * diff[axis];
                //std::cout<<diff[axis]<<std::endl;
            }

            this->aggregates[i]->remove_constituent_from_cell();
            this->aggregates[i]->move(temp_dr);
            this->aggregates[i]->add_constituent_to_cell();        
        }

    }

    //std::cout<<"max_overlap="<<max_overlap<<std::endl;


    /*if ((iters > 2) && (max_overlap < (-1. * 1e-4))){

        //std::cout<<"max_overlap = "<<max_overlap<<std::endl;

        std::cout<<"particle_id = "<<p_id<<"\t neighbour_id = "<<n_id<<"\t max_overlap = "<<max_overlap<<std::endl;

        for (int i = 0; i < this->D; i++)
            std::cout<<dr[i]<<"\t";

        std::cout<<"\n";

        //std::cout<<particle_id<<"\t"<<neighbour_id<<std::endl;

        for (int axis = 0; axis < this->D; axis++){
            dr[axis] = (max_overlap * dr[axis]);
        }

        for (int i = 0; i < this->D; i++)
            std::cout<<dr[i]<<"\t";

        std::cout<<"\n";

        exit(EXIT_FAILURE);

    }*/




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