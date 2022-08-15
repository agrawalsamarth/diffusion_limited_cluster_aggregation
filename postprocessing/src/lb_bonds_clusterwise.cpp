#include <post.h>

namespace post_p
{

void postprocessing::dump_lb_bonds_for_cluster_via_invA()
{
    determine_LB_bonds_clusterwise();
}

bool postprocessing::check_if_particles_placed()
{

    bool all_particles_placed = true;

    for (int i = 0; i < numParticles(); i++) {
        all_particles_placed = all_particles_placed && is_placed(i);
    }

    //std::cout<<"bool values = "<<all_particles_placed<<std::endl;

    return all_particles_placed;

}

void postprocessing::determine_LB_bonds_clusterwise()
{

    reset_unfolding_params();
    build_bond_map();
    reset_bond_map(true);

    double max_length;
    int max_row, max_col;

    int total_lbp = 0;
    //int totalClusters;
    ref_pos.resize(dim());
    totalClusters_ = 0;


    while (!check_if_particles_placed()) {

        num_bonds_for_cluster = 0;
        num_particles_for_cluster = 0;
        unique_bonds.clear();
        index_to_particles.clear();
        particles_to_index.clear();

        for (int i = 0; i < numParticles(); i++){
            if (is_placed(i) == false){
                particles_to_index[i] = 0;
                index_to_particles[0] = i;
                num_particles_for_cluster += 1;
                unfold_for_clusterwise(i,i);
                break;
            }
        }

        //std::cout<<"num_particles_for_cluster = "<<num_particles_for_cluster<<std::endl;
        //std::cout<<"num_bonds_for_cluster = "<<num_bonds_for_cluster<<std::endl;

        modified_folded_x.resize(num_particles_for_cluster,dim());
        copy_positions_for_cluster();

        A.resize((num_particles_for_cluster-1), (num_particles_for_cluster-1));
        b.resize((num_particles_for_cluster-1));
        x.resize((num_particles_for_cluster-1));
        bond_lengths_direction_wise.resize(num_bonds_for_cluster, dim());
        bond_lengths.resize(num_bonds_for_cluster);

        do {

            A.setZero();
            modify_coords_for_cluster();
            build_A_for_cluster();

            if (!A.isCompressed()){
                A.makeCompressed();
            }

            solver.analyzePattern(A);
            solver.factorize(A);

            for (int axis = 0; axis < dim(); axis++){

                b.setZero();
                build_b_for_cluster(axis);
                x = solver.solve(b);
                modify_coords_after_minimization_for_cluster(axis);

            }

            calculate_bond_lengths_for_cluster();
            max_length = bond_lengths.maxCoeff(&max_row);

            //std::cout<<"max_length = "<<max_length<<std::endl;

            if (max_length > 1e-10){

                switch_off_bonds({index_to_particles[unique_bonds[max_row].first], index_to_particles[unique_bonds[max_row].second]});
                std::cout<<totalClusters_<<","<<index_to_particles[unique_bonds[max_row].first]<<"-"<<index_to_particles[unique_bonds[max_row].second]<<std::endl;
                total_lbp++;

            }


        } while(max_length > 1e-10);

        totalClusters_++;

    }

}

void postprocessing::unfold_for_clusterwise(const int prev, const int next)
{

    for (int axis = 0; axis < dim(); axis++)
        posDiff(axis) = get_periodic_image(pos(next,axis) - pos(prev,axis), axis);

    for (int axis = 0; axis < dim(); axis++){
        if (prev == next)
            unfolded_coords(next,axis) = pos(prev,axis) + posDiff(axis);
        else
            unfolded_coords(next,axis) = unfolded_coords(prev,axis) + posDiff(axis);
    }

    is_placed(next) = true;

    for (int att = 0; att < numAttachments(next); att++)
    {
        temp_next = attachment(next, att);
        
        
        if (is_placed(temp_next) == false){

            particles_to_index[temp_next] = num_particles_for_cluster;
            index_to_particles[num_particles_for_cluster] = temp_next;
            num_particles_for_cluster++;

            /*if (particles_to_index.count(next) != 1){
                std::cout<<"unfold 1"<<std::endl;
            }


            if (particles_to_index.count(temp_next) != 1){
                std::cout<<"unfold 2"<<std::endl;
            }*/            

            index_i = particles_to_index[next];
            index_j = particles_to_index[temp_next];

            if (std::find(unique_bonds.begin(), unique_bonds.end(), std::make_pair(index_j,index_i)) == unique_bonds.end()){
                unique_bonds.push_back({index_i,index_j});
                num_bonds_for_cluster += 1;
            }

            unfoldedAttachments(next)      = unfoldedAttachments(next) + 1;
            unfoldedAttachments(temp_next) = unfoldedAttachments(temp_next) + 1;
            unfold_for_clusterwise(next, temp_next);
        }

        else {

            index_i = particles_to_index[next];
            index_j = particles_to_index[temp_next];

            if (std::find(unique_bonds.begin(), unique_bonds.end(), std::make_pair(index_j,index_i)) == unique_bonds.end()){
                unique_bonds.push_back({index_i,index_j});
                num_bonds_for_cluster += 1;
            }            
            
            for (int axis = 0; axis < dim(); axis++) {
                posDiff(axis) = unfolded_coords(temp_next, axis) - unfolded_coords(next, axis);
                if ((posDiff(axis) > halfL(axis)) || (posDiff(axis) < -halfL(axis))){
                    cluster_percolation(totalClusters_,axis) = 1;
                    load_bearing_paths_[axis]++;
                }
            }

        }

    }


}

void postprocessing::copy_positions_for_cluster()
{

    int index;

    for (int i = 0; i < num_particles_for_cluster; i++){
        for (int axis = 0; axis < dim(); axis++){

            index = index_to_particles[i];

            modified_folded_x(i,axis)  = pos(index,axis);
            modified_folded_x(i,axis) -= L(axis) * round(modified_folded_x(i,axis)/L(axis)); 

        }
    }

}

void postprocessing::modify_coords_for_cluster()
{

    int index;

    for (int axis = 0; axis < dim(); axis++){
        ref_pos(axis) = modified_folded_x(num_particles_for_cluster-1, axis);
    }

    for (int i = 0; i < num_particles_for_cluster; i++){

        for (int axis = 0; axis < dim(); axis++){

            modified_folded_x(i,axis)  = modified_folded_x(i,axis) - ref_pos(axis);
            modified_folded_x(i,axis) -= L(axis) * round(modified_folded_x(i,axis)/L(axis));

            //std::cout<<pos(i,axis)<<" ";

        }

        //std::cout<<"\n";


    }


}

void postprocessing::modify_coords_after_minimization_for_cluster(int axis)
{

    for (int i = 0; i < num_particles_for_cluster-1; i++)
        modified_folded_x(i,axis)  = (L(axis) * x(i)) + ref_pos(axis);

    modified_folded_x(num_particles_for_cluster-1,axis) = ref_pos(axis);

    for (int i = 0; i < num_particles_for_cluster; i++)
        modified_folded_x(i,axis) -= L(axis) * round(modified_folded_x(i,axis)/L(axis));


}

void postprocessing::build_A_for_cluster()
{

    int i,j;
    int index_i, index_j;

    for (int index = 0; index < num_bonds_for_cluster; index++){

        i = unique_bonds[index].first;
        j = unique_bonds[index].second;

        index_i = index_to_particles[i];
        index_j = index_to_particles[j];

        /*if (bond_map_status.count({index_i,index_j}) == 0){
            std::cout<<"problem with A"<<std::endl;
        }*/

        if (bond_map_status[{index_i,index_j}] == 1){

            if (i == (num_particles_for_cluster-1)){
                A.coeffRef(j,j) -= 1;
            }

            else if (j == (num_particles_for_cluster-1)){
                A.coeffRef(i,i) -= 1;
            }

            else{

                A.coeffRef(i,j) += 1;
                A.coeffRef(j,i) += 1;
                A.coeffRef(i,i) -= 1;
                A.coeffRef(j,j) -= 1;

            }

        }


    }

}

void postprocessing::build_b_for_cluster(int axis)
{

    int i,j;
    int index_i, index_j;
    double dx;

    for (int index = 0; index < num_bonds_for_cluster; index++){

        i = unique_bonds[index].first;
        j = unique_bonds[index].second;

        index_i = index_to_particles[i];
        index_j = index_to_particles[j];

        /*if (bond_map_status.count({index_i,index_j}) == 0){
            std::cout<<"problem with b"<<std::endl;
        }*/

        if (bond_map_status[{index_i,index_j}] == 1){

            dx = round((modified_folded_x(i,axis) - modified_folded_x(j,axis))/L(axis));

            if (i == (num_particles_for_cluster-1)){
                b(j) += dx;
            }

            else if (j == (num_particles_for_cluster-1)){
                b(i) -= dx;
            }

            else {
                b(i) -= dx;
                b(j) += dx;
            }

        }


    }

}

void postprocessing::calculate_bond_lengths_direction_wise_for_cluster(int axis)
{

    //bond_distance.setZero();
    int i,j;
    int index_i, index_j;

    for (int index = 0; index < num_bonds_for_cluster; index++){

        i = unique_bonds[index].first;
        j = unique_bonds[index].second;

        index_i = index_to_particles[i];
        index_j = index_to_particles[j];

        if (bond_map_status[{index_i,index_j}] == 1){
            //bond_lengths(index) = get_periodic_image(pos(i,axis)-pos(j,axis), axis) * get_periodic_image(pos(i,axis)-pos(j,axis), axis);
            bond_lengths_direction_wise(index, axis)  = (modified_folded_x(i, axis) - modified_folded_x(j, axis));
            bond_lengths_direction_wise(index, axis) -= (L(axis) * round(bond_lengths_direction_wise(index, axis)/L(axis)));
            bond_lengths_direction_wise(index, axis) *= bond_lengths_direction_wise(index, axis); 
        }

        else {
            bond_lengths_direction_wise(index, axis)  = 0.;
        }

    }


}

void postprocessing::calculate_bond_lengths_for_cluster()
{

    bond_lengths_direction_wise.setZero();
    bond_lengths.setZero();

    for (int axis = 0; axis < dim(); axis++){
        calculate_bond_lengths_direction_wise_for_cluster(axis);
    }

    for (int i = 0; i < num_bonds_for_cluster; i++){
        for (int axis = 0; axis < dim(); axis++) {

            bond_lengths(i) += bond_lengths_direction_wise(i,axis);

        }

        bond_lengths(i) = sqrt(bond_lengths(i));

    }


}

    
}