#include <post.h>

namespace post_p
{


void postprocessing::dump_stochastic_ray_tracing(char *filename, int num_rays)
{
    f_srt = fopen(filename, "w");
    stochastic_ray_tracing(filename, num_rays);
    fclose(f_srt);

    free(delta_x);
    free(inv_delta_x);
    free(nx);
    free(idx_multiplicator);  

}



void postprocessing::discretize_domain()
{

    delta_x     = (double*)malloc(sizeof(double)*dim());
    inv_delta_x = (double*)malloc(sizeof(double)*dim());
    nx          = (int*)malloc(sizeof(int)*dim());
    idx_multiplicator = (int*)malloc(sizeof(int)*dim());

    for (int axis = 0; axis < dim(); axis++){
        nx[axis] = (int)(L(axis));
    }

    for (int axis = 0; axis < dim(); axis++){
        delta_x[axis]     = L(axis)/nx[axis];
        inv_delta_x[axis] = 1./delta_x[axis]; 
    }

    int total_nx = 1;

    for (int axis = 0; axis < dim(); axis++){
        total_nx *= nx[axis];
        idx_multiplicator[axis] = 1;
    }

    for (int axis = 0; axis < dim(); axis++){
        for (int i = axis; i < (dim()-1); i++)
            idx_multiplicator[axis] *= nx[axis];
    }

    grid_config.resize(total_nx);

}

int postprocessing::get_idx_from_coords(std::vector<int> val)
{
    int sum = 0;
    for (int axis = 0; axis < dim(); axis++)
        sum += val[axis]*idx_multiplicator[axis];
    return sum;
}


void postprocessing::build_neighbour_lists()
{

    std::vector<int> coords;
    int index;
    coords.resize(dim());

    for (int i = 0; i < numParticles(); i++){
        for (int axis = 0; axis < dim(); axis++)
            coords[axis] = (int)(pos(i,axis) * inv_delta_x[axis]);

        index = get_idx_from_coords(coords);
        grid_config[index].push_back(i);
    }

    /*for (int i = 0; i < grid_config.size(); i++){
        std::cout<<"index="<<i<<"\n";
        for (int j = 0; j < grid_config[i].size(); j++)
            std::cout<<grid_config[i][j]<<std::endl;
        std::cout<<"----------------------\n";
    }*/

}

void postprocessing::get_neighbours(std::vector<int> &all_neighs, int *init_grid)
{

    all_neighs.clear();
    std::vector<int> temp_coords;
    temp_coords.resize(dim());

    int x_temp;
    int y_temp;
    int idx;

    /*std::cout<<"---------init_grid----------------\n";
    std::cout<<init_grid[0]<<" "<<init_grid[1]<<" "<<init_grid[2]<<"\n";
    //std::cout<<x_temp<<" "<<y_temp<<" "<<k<<"\n";
    std::cout<<"----------finit grid-----------------\n";*/


    for (int x_diff = -1; x_diff <= 1; x_diff++){

        x_temp  = (init_grid[0]+x_diff);
        x_temp += nx[0]*(x_temp < 0) - nx[0]*(x_temp >= nx[0]);

        temp_coords[0] = x_temp;
        
        for (int y_diff = -1; y_diff <= 1; y_diff++){


            y_temp  = (init_grid[1]+y_diff);
            y_temp += nx[1]*(y_temp < 0) - nx[1]*(y_temp >= nx[1]);

            temp_coords[1] = y_temp;

            for (int k = 0; k < nx[2]; k++){

                temp_coords[2] = k;
                idx = get_idx_from_coords(temp_coords);
                all_neighs.insert(all_neighs.end(), grid_config[idx].begin(), grid_config[idx].end());

                //std::cout<<temp_coords[0]<<" "<<temp_coords[1]<<" "<<temp_coords[2]<<"\n";
                //std::cout<<idx<<"\n";

                /*std::cout<<"-----------------------------------\n";
                std::cout<<x_diff<<" "<<y_diff<<" "<<k<<"\n"; 
                std::cout<<x_temp<<" "<<y_temp<<" "<<k<<"\n";               
                std::cout<<"-----------------------------------\n";*/


                /*for (int tmp_idx = 0; tmp_idx < grid_config[idx].size(); tmp_idx++){
                    grid_config
                    all_neighs.push_back(grid_config[idx][tmp_idx]);
                }*/

                /*for (int tmp_idx = 0; tmp_idx < all_neighs.size(); tmp_idx++){
                    
                }*/


            }



        }


    }


}

bool postprocessing::calculate_quad_eqn(double *sph_pos, double *temp_pos, std::pair<double, double> &tc)
{
    //std::pair<double, double> tc;
    double discriminant = 0.;
    double q = 0.;
    double p = 0.;
    double diff = 0.;

    q = get_periodic_image(sph_pos[2]-temp_pos[2], 2);

    for (int axis = 0; axis < dim(); axis++)
        diff += get_periodic_image(sph_pos[axis]-temp_pos[axis], axis)*get_periodic_image(sph_pos[axis]-temp_pos[axis], axis);


    discriminant = (q*q) - diff + 0.25;

    if (discriminant < 0.){
        return false;
    }

    else{
        tc.first  = q - sqrt(discriminant);
        tc.second = q + sqrt(discriminant);
        return true;
        
        //if (tc.first > tc.second)
            //std::cout<<"kya hagg ke rakho ho bhai"<<std::endl;

    }

    //return tc;

}

void postprocessing::print_chord_lengths(char *filename, std::vector<std::pair<double, double>> tc_list)
{
    
    double tmp_offset;

    if (tc_list.size() == 0){
        fprintf(f_srt, "NaN\n");
        //std::cout<<"yahaan aaya chutiye"<<std::endl;
    }

    else {


        //std::sort(tc_list.begin(), tc_list.end(), [](const std::pair<double, double>& a, const std::pair<double, double>& b) {return a.first < b.first;});

        if (tc_list[0].first < 0.){
            tmp_offset = tc_list[0].first;
            for (int i = 0; i < tc_list.size(); i++){
                tc_list[i].first  -= tmp_offset;
                tc_list[i].second -= tmp_offset;
            }
        }

        for (int i = 0; i < (tc_list.size()-1); i++){
            //fprintf(f_srt, "%lf,", tc_list[i+1].first-tc_list[i].second);
            //std::cout<<"i="<<i<<std::endl;
            //fprintf(f_srt, "%lf,%lf,", tc_list[i].first,tc_list[i].second);
            fprintf(f_srt, "%lf,", tc_list[i].second-tc_list[i].first);


        }

        //fprintf(f_srt, "%lf,%lf\n", tc_list[tc_list.size()-1].first,tc_list[tc_list.size()-1].second);
        fprintf(f_srt, "%lf\n", tc_list[tc_list.size()-1].second-tc_list[tc_list.size()-1].first);

        //std::cout<<"arrey gandu"<<std::endl;
        //std::cout<<tc_list[0].first<<" "<<tc_list[0].second<<std::endl;
        //fprintf(f_srt, "%lf\n", tc_list[0].first-tc_list[tc_list.size()-1].second+L(0));


    }



}

void postprocessing::stochastic_ray_tracing(char *filename, int num_rays)
{
    discretize_domain();
    build_neighbour_lists();

    std::vector<int> all_neighs;
    std::pair<double, double> tc;
    std::vector<std::pair<double, double>> tc_list;
    double *temp_pos;
    double *sph_pos;
    int    *int_grid;
    bool    sol;


    temp_pos = (double*)malloc(sizeof(double)*dim());
    sph_pos  = (double*)malloc(sizeof(double)*dim());
    int_grid = (int*)malloc(sizeof(int)*dim());


    for (int i = 0; i < num_rays; i++) {

        tc_list.clear();

        temp_pos[0] = L(0)*dis(generator);
        temp_pos[1] = L(1)*dis(generator);
        temp_pos[2] = 0.;

        for (int axis = 0; axis < dim(); axis++)
            int_grid[axis] = inv_delta_x[axis]*temp_pos[axis];

        get_neighbours(all_neighs, int_grid);


        /*std::cout<<"size = "<<all_neighs.size()<<std::endl;
        for (int i = 0; i < all_neighs.size(); i++)
            std::cout<<all_neighs[i]<<std::endl;
        std::cout<<"-------------------------------------------\n";*/

        for (int id = 0; id < all_neighs.size(); id++){

            for (int axis = 0; axis < dim(); axis++)
                sph_pos[axis] = pos(all_neighs[id], axis);

            
            sol = calculate_quad_eqn(sph_pos, temp_pos, tc);

            if (sol){                
                tc_list.push_back(tc);

                /*for (int axis = 0; axis < dim(); axis++){
                    std::cout<<(int)(sph_pos[axis]*inv_delta_x[axis])<<" ";
                }
                std::cout<<"\n";*/
            }


        }

        print_chord_lengths(filename, tc_list);
        
    }

    free(sph_pos);
    free(int_grid);
    free(temp_pos);

}



}