#include <post.h>

namespace post_p
{

void postprocessing::makeCombiUtil(int n, int left, int k)
{
	// Pushing this vector to a vector of vector
	if (k == 0) {
		ans.push_back(tmp);
		return;
	}

	// i iterates from left to n. First time
	// left will be 1
	for (int i = left; i < n; ++i)
	{
		tmp.push_back(i);
		makeCombiUtil(n, i + 1, k - 1);

		// Popping out last inserted element
		// from the vector
		tmp.pop_back();
	}
}

// Prints all combinations of size k of numbers
// from 1 to n.
void postprocessing::makeCombi(int n, int k)
{
	makeCombiUtil(n, 0, k);
}


void postprocessing::build_bond_map()
{

    for (int i = 0; i < numParticles(); i++){

        for (int j = 0; j < numAttachments(i); j++)
            bond_map_status.insert({{i,attachment(i,j)}, 1});
    }

}

void postprocessing::switch_off_bonds(std::pair<int,int> bond)
{

    bond_map_status[bond] = 0;

    temp_pair.first = bond.second;
    temp_pair.second = bond.first;

    bond_map_status[temp_pair] = 0;

}

void postprocessing::switch_on_bonds(std::pair<int,int> bond)
{

    bond_map_status[bond] = 1;

    temp_pair.first = bond.second;
    temp_pair.second = bond.first;

    bond_map_status[temp_pair] = 1;

}

void postprocessing::activate_path(std::vector<std::pair<int,int>> bonds, bool status)
{

    for (int i = 0; i < bonds.size(); i++){

        if (status)
            switch_on_bonds(bonds[i]);

        else
            switch_off_bonds(bonds[i]);
        

    }

}

void postprocessing::reset_bond_map(bool status)
{

    if (status) {

        for (int i = 0; i < numParticles(); i++){
            for (int j = 0; j < numAttachments(i); j++) {

                switch_on_bonds({i, attachment(i,j)});

            }
        }
        
    }

    else {

        for (int i = 0; i < numParticles(); i++){
            for (int j = 0; j < numAttachments(i); j++) {

                switch_off_bonds({i, attachment(i,j)});

            }
        }       

    }


}

void postprocessing::create_subsystem()
{

    reset_bond_map(false);

    for (int i = 0; i < current_path.size(); i++)
        switch_on_bonds(current_path[i]);


}

void postprocessing::erase_subsystem()
{

    reset_bond_map(true);

    for (int i = 0; i < final_percolating_bonds.size(); i++){

        if (pb_status[i]) {

            for (int j = 0; j < final_percolating_bonds[i].size(); j++) {

                switch_off_bonds(final_percolating_bonds[i][j]);

            }

        }
    }


}

void postprocessing::switch_off_alt_lbp(int index)
{

    for (int i = 0; i < final_percolating_bonds.size(); i++){

        if ((i != index) && (pb_status[i])) {
        //if (i != index) {

            for (int j = 0; j < final_percolating_bonds[i].size(); j++)
                switch_off_bonds(final_percolating_bonds[i][j]);

        }

    }


}

void postprocessing::reset_unfolding_params()
{

    for (int j = 0; j < numParticles(); j++){

        is_placed(j) = false;

        for (int axis = 0; axis < dim(); axis++)
            unfolded_coords(j,axis) = 0.;

    }

    for (int axis = 0; axis < dim(); axis++)
        temp_lbp_[axis] = 0;

}

void postprocessing::print_lbp()
{

    /*for (int i = 0; i < final_percolating_bonds.size(); i++){

        std::cout<<"lbp "<<i<<" = \t";

        for (int j = 0; j < final_percolating_bonds[i].size(); j++) {

            std::cout<<final_percolating_bonds[i][j].first<<"-"<<final_percolating_bonds[i][j].second<<"\t";

        }

        std::cout<<"\n";

    }*/

    for (int i = 0; i < weak_links.size(); i++){

        std::cout<<"lbp "<<i<<" = \t";

        for (int j = 0; j < weak_links[i].size(); j++) {

            std::cout<<weak_links[i][j].first<<"-"<<weak_links[i][j].second<<"\t";

        }

        std::cout<<"\n";

    }

}

void postprocessing::isolation_routine()
{

    reset_unfolding_params();

    for (int i = 0; i < numParticles(); i++){

        if (is_placed(i) == false){

            path_percolation = false;
            path_components.clear();
            current_path.clear();
            unfold_for_lbp(i,i,true);

            if (path_percolation == true){
                create_subsystem();
                percolation_detection();
                path_percolation = true;
                total_lbp++;
                erase_subsystem();
                break;
            }

        }


    }

}

void postprocessing::init_unfolding_for_lbp()
{

    build_bond_map();
    martin_test();

    /*bool build_pb = false;
    int ref_axis = 0;
    total_lbp = 0;

    for (int i = 0; i < numParticles(); i++){
        for (int axis = 0; axis < dim(); axis++){
            cluster_percolation(i,axis) = 0;
        }
    }

    do {

        isolation_routine();

    } while (path_percolation);

    postprocess_lbp();*/
    //print_lbp();

}

void postprocessing::percolation_detection()
{
    percolating_bonds.clear();
    int size;

    for (int index = 0; index < current_path.size(); index++){

        reset_unfolding_params();
        switch_off_bonds(current_path[index]);

        path_percolation = false;

        for (int i = 0; i < path_components.size(); i++){

            if (is_placed(path_components[i]) == false){

                unfold_for_lbp(path_components[i],path_components[i],false);

                if (path_percolation == true)
                    break;
                

            }

        }

        if (path_percolation == false)
            percolating_bonds.push_back(current_path[index]);

        switch_on_bonds(current_path[index]);

    }

    //pb_status[final_percolating_bonds.size()] = 1;
    //std::cout<<"size = "<<final_percolating_bonds.size()<<std::endl;
    final_percolating_bonds.push_back(percolating_bonds);
    pb_status.push_back(true);
    

}

void postprocessing::shred_path(int index)
{

    reset_bond_map(true);
    switch_off_alt_lbp(index);
    switch_off_bonds(final_percolating_bonds[index][0]);
    isolation_routine();

    for (int i = 0; i < final_percolating_bonds[index].size(); i++)
        std::cout<<final_percolating_bonds[index][i].first<<"-"<<final_percolating_bonds[index][i].second<<",";

    std::cout<<"\n";

    index = final_percolating_bonds.size() - 1;

    for (int i = 0; i < final_percolating_bonds[index].size(); i++)
        std::cout<<final_percolating_bonds[index][i].first<<"-"<<final_percolating_bonds[index][i].second<<",";

    std::cout<<"\n";

    do {

        isolation_routine();

    } while (path_percolation);


}

void postprocessing::postprocess_lbp()
{

    bool active_flag;

    for (int i = 0; i < final_percolating_bonds.size(); i++){

        //std::cout<<"size = "<<final_percolating_bonds.size()<<std::endl;

        reset_bond_map(true);
        percolating_bonds.clear();
        switch_off_alt_lbp(i);
        active_flag = true;

        for (int j = 0; j < final_percolating_bonds[i].size(); j++) {

            switch_off_bonds(final_percolating_bonds[i][j]);
            reset_unfolding_params();
            path_percolation = false;

            for (int k = 0; k < numParticles(); k++){

                if (is_placed(k) == false){

                    unfold_for_lbp(k,k,false);

                    if (path_percolation == true)
                        break;
                    

                }

            }

            if (path_percolation == false) {
                percolating_bonds.push_back(final_percolating_bonds[i][j]);
                active_flag = false;
            }

            switch_on_bonds(final_percolating_bonds[i][j]);


        }

        if (active_flag) {
            pb_status[i] = false;
            shred_path(i);
        }

        else {
            weak_links.push_back(percolating_bonds);
        }

    }


}

void postprocessing::unfold_for_lbp(const int prev, const int next, bool build_pb)
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

    if (build_pb)
        path_components.push_back(next);

    for (int att = 0; att < numAttachments(next); att++)
    {

        //if (path_percolation)
            //break;

        temp_next = attachment(next, att);

        if (bond_map_status[{next, temp_next}] == 1){

            if ((build_pb == true)) {

                    if(std::find(current_path.begin(), current_path.end(), std::make_pair(temp_next, next)) == current_path.end()) {
                        current_path.push_back({next, temp_next});
                    } 

            }
        
            if (is_placed(temp_next) == false){
                unfoldedAttachments(next)      = unfoldedAttachments(next) + 1;
                unfoldedAttachments(temp_next) = unfoldedAttachments(temp_next) + 1;
                unfold_for_lbp(next, temp_next, build_pb);
            }

            else {
                
                for (int axis = 0; axis < dim(); axis++) {

                    posDiff(axis) = unfolded_coords(temp_next, axis) - unfolded_coords(next, axis);

                    if ((posDiff(axis) > halfL(axis)) || (posDiff(axis) < -halfL(axis))){
                        cluster_percolation(total_lbp,axis) = 1;
                        temp_lbp_[axis]++;
                        path_percolation = true;
                    }

                }

            }

        }

    }

}

void postprocessing::martin_test()
{

    reset_bond_map(true);

    std::vector<std::pair<int,int>> test_bonds;

    test_bonds.push_back({170,3101});
    test_bonds.push_back({483,736});
    test_bonds.push_back({1477,1905});
    test_bonds.push_back({683,1700});
    //test_bonds.push_back({1965,2576});

    for (int i = 0; i < test_bonds.size(); i++)
        switch_off_bonds(test_bonds[i]);

    path_percolation = false;
    reset_unfolding_params();


    for (int i = 0; i < numParticles(); i++){

        if (is_placed(i) == false){
            std::cout<<"i="<<i<<std::endl;
            unfold_for_lbp(i, i, false);
        }

        if (path_percolation)
            std::cout<<"still percolating"<<std::endl;

    }

    if (!path_percolation)
        std::cout<<"no percolation at all"<<std::endl;


}

void postprocessing::lbp_brute_force()
{

    reset_bond_map(true);
    std::vector<std::pair<int,int>> unique_bonds;

    int num_bonds = 0;
    int bond_sum = 0;

    for (int i = 0; i < numParticles(); i++){
        for (int j = 0; j < numAttachments(i); j++){

            if (std::find(unique_bonds.begin(), unique_bonds.end(), std::make_pair(attachment(i,j),i)) == unique_bonds.end()) {

                unique_bonds.push_back({i, attachment(i,j)});
                num_bonds++;

            }

        }
    }

    /*for (int i = 1; i <= 2; i++){
        std::cout<<"i = "<<i<<std::endl;

        tmp.clear();
        ans.clear();

        makeCombi(num_bonds, i);

        for (int j = 0; j < ans.size(); j++){
            for (int k = 0; k < ans[i].size(); k++){
                std::cout<<ans[j][k]<<"\t";
            }
            std::cout<<"\n";
        }

    }*/


    for (int i = 1; i < num_bonds; i++) {

        tmp.clear();
        ans.clear();

        makeCombi(num_bonds, i);

        for (int j = 0; j < ans.size(); j++){

            reset_bond_map(true);
         
            for (int k = 0; k < ans[j].size(); k++){

                switch_off_bonds(unique_bonds[ans[j][k]]);
                
            }

            path_percolation = false;
            reset_unfolding_params();

            for (int index = 0; index < numParticles(); index++){

                if (is_placed(index) == false){
                    unfold_for_lbp(index, index, false);
                }

            }

            //std::cout<<"pp = "<<path_percolation<<std::endl;

            if (!path_percolation){

                for (int k = 0; k < ans[j].size(); k++)
                    std::cout<<unique_bonds[ans[j][k]].first<<"-"<<unique_bonds[ans[j][k]].second<<"\n";

                break;

            }

        }

        if (!path_percolation){
            break;
        }


    }



}

}


/*void postprocessing::percolation_detection()
{

    int lbp_min;
    int i_min;
    int bond_sum;

    for (int axis = 0; axis < dim(); axis++){

        lbp_min = load_bearing_paths(10,axis);
        i_min=10;

        /*for (int i = 0; i < numParticles(); i++){
            
            if (load_bearing_paths(i,axis) < lbp_min){
                lbp_min = load_bearing_paths(i,axis);
                i_min   = i;
            }

        }

        if (lbp_min > 0) {

            for (int j = 0; j < numParticles(); j++){
                is_placed(j) = false;
                unfoldedAttachments(j) = 0;
                for (int axis_2 = 0; axis_2 < dim(); axis_2++)
                    unfolded_coords(j,axis_2) = 0.;
            }

            for (int axis_2 = 0; axis_2 < dim(); axis_2++)
                temp_lbp_[axis_2] = 0;

            percolating_bonds.clear();
            unfold_for_lbp(i_min,i_min, true, axis);

            lbp_min = percolating_bonds.size();

            std::cout<<"lbp_min = "<<lbp_min<<std::endl;

            divisors      = (int*)malloc(sizeof(int) * lbp_min);
            bond_bit_repr = (int*)malloc(sizeof(int) * lbp_min);

            for (int i = 0; i < lbp_min; i++)
                divisors[i] = std::pow(2, (lbp_min-1-i));


            //while (temp_lbp_[axis] != 0){
            for (int i = (std::pow(2,lbp_min)-1); i >= 0; i--) {

                for (int j = 0; j < lbp_min; j++)
                    bond_bit_repr[j] = (i/divisors[j])%2;

                /*std::cout<<"i = "<<i<<"\t";

                for (int j = 0; j < lbp_min; j++)
                    std::cout<<bond_bit_repr[j];

                std::cout<<"\n";

                for (int j = 0; j < lbp_min; j++) {

                    bond_map_status[{percolating_bonds[j].first, percolating_bonds[j].second}] = bond_bit_repr[j];
                    bond_map_status[{percolating_bonds[j].second, percolating_bonds[j].first}] = bond_bit_repr[j];

                }

                for (int j = 0; j < numParticles(); j++){
                    is_placed(j) = false;
                    unfoldedAttachments(j) = 0;
                    for (int axis_2 = 0; axis_2 < dim(); axis_2++)
                        unfolded_coords(j,axis_2) = 0.;
                }

                for (int axis_2 = 0; axis_2 < dim(); axis_2++)
                    temp_lbp_[axis_2] = 0;

                unfold_for_lbp(i_min, i_min, false, axis);

                /*for (int j = 0; j < numParticles(); j++){

                    if (is_placed(j) == false)
                        std::cout<<"problem here"<<std::endl;

                }


                if (temp_lbp_[axis] == 0){

                    bond_sum = 0;

                    for (int j = 0; j < lbp_min; j++)
                        bond_sum += bond_bit_repr[j];

                    std::cout<<"lbp = "<<(lbp_min-bond_sum)<<std::endl;

                    for (int j = 0; j < lbp_min; j++) {

                        bond_map_status[{percolating_bonds[j].first, percolating_bonds[j].second}] = 1;
                        bond_map_status[{percolating_bonds[j].second, percolating_bonds[j].first}] = 1;

                    }

                    for (int j = 0; j < percolating_bonds.size(); j++)
                        std::cout<<"p = "<<percolating_bonds[j].first<<"\t"<<percolating_bonds[j].second<<std::endl;
                    

                    break;

                }

                for (int j = 0; j < lbp_min; j++) {

                    bond_map_status[{percolating_bonds[j].first, percolating_bonds[j].second}] = 1;
                    bond_map_status[{percolating_bonds[j].second, percolating_bonds[j].first}] = 1;

                }

                /*std::cout<<"i = "<<i<<"\t";

                for (int j = 0; j < lbp_min; j++)
                    std::cout<<bond_bit_repr[j];

                std::cout<<"\t axis = "<<axis<<"\t lbp = "<<temp_lbp_[axis]<<std::endl;



            }

        }

    }

}


void postprocessing::percolation_detection()
{

    int lbp_min = 0;
    int i_min;
    int bond_sum;
    int lbp_min_axis;

    percolating_bonds.clear();

    for (int axis = 0; axis < dim(); axis++){

        lbp_min = load_bearing_paths(0,axis);
        i_min=0;

        for (int i = 0; i < numParticles(); i++){
            
            if (load_bearing_paths(i,axis) < lbp_min){
                lbp_min = load_bearing_paths(i,axis);
                i_min   = i;
            }

        }

        for (int j = 0; j < numParticles(); j++){
            is_placed(j) = false;
            unfoldedAttachments(j) = 0;
            for (int axis_2 = 0; axis_2 < dim(); axis_2++)
                unfolded_coords(j,axis_2) = 0.;
        }

        for (int axis_2 = 0; axis_2 < dim(); axis_2++)
            temp_lbp_[axis_2] = 0;


        unfold_for_lbp(i_min,i_min, true, axis);

    }

    lbp_min = percolating_bonds.size();

    //std::cout<<"lbp_min="<<lbp_min<<std::endl;

    if (lbp_min > 0) {


        divisors      = (int*)malloc(sizeof(int) * lbp_min);
        bond_bit_repr = (int*)malloc(sizeof(int) * lbp_min);

        for (int i = 0; i < lbp_min; i++)
            divisors[i] = std::pow(2, (lbp_min-1-i));


        //while (temp_lbp_[axis] != 0){
        for (int i = (std::pow(2,lbp_min)-1); i >= 0; i--) {

            for (int j = 0; j < lbp_min; j++)
                bond_bit_repr[j] = (i/divisors[j])%2;

            /*std::cout<<"i = "<<i<<"\t";

            for (int j = 0; j < lbp_min; j++)
                std::cout<<bond_bit_repr[j];

            std::cout<<"\n";

            for (int j = 0; j < lbp_min; j++) {

                bond_map_status[{percolating_bonds[j].first, percolating_bonds[j].second}] = bond_bit_repr[j];
                bond_map_status[{percolating_bonds[j].second, percolating_bonds[j].first}] = bond_bit_repr[j];

            }

            for (int j = 0; j < numParticles(); j++){
                is_placed(j) = false;
                unfoldedAttachments(j) = 0;
                for (int axis_2 = 0; axis_2 < dim(); axis_2++)
                    unfolded_coords(j,axis_2) = 0.;
            }

            for (int axis_2 = 0; axis_2 < dim(); axis_2++)
                temp_lbp_[axis_2] = 0;

            unfold_for_lbp(i_min, i_min, false, 0);

            for (int j = 0; j < numParticles(); j++){

                if (is_placed(j) == false)
                    std::cout<<"problem here"<<std::endl;

            }

            lbp_min_axis = 0;

            for (int axis = 0; axis < dim(); axis++)
                lbp_min_axis += temp_lbp_[axis];

            if (lbp_min_axis == 0){

                bond_sum = 0;

                for (int j = 0; j < lbp_min; j++)
                    bond_sum += bond_bit_repr[j];

                std::cout<<"lbp = "<<(lbp_min-bond_sum)<<std::endl;

                for (int j = 0; j < lbp_min; j++) {

                    bond_map_status[{percolating_bonds[j].first, percolating_bonds[j].second}] = 1;
                    bond_map_status[{percolating_bonds[j].second, percolating_bonds[j].first}] = 1;

                }

                break;

            }


        }

    }

    

}*/

