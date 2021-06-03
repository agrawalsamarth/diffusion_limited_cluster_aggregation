#include "dlca_lattice_3d.h"

std::vector<std::string> dlca_lattice_3d::split_string_by_delimiter(const std::string& s, char delimiter)
{
   std::vector<std::string> tokens;
   std::string token;
   std::istringstream tokenStream(s);
   while (std::getline(tokenStream, token, delimiter))
   {
      tokens.push_back(token);
   }
   return tokens;
}

void dlca_lattice_3d::read_params_parser(char *params_name)
{

    std::ifstream parser(params_name, std::ifstream::in);
    int count = 0;

    std::string str;
    std::vector<std::string> results;

    while (getline(parser, str)){

        results = split_string_by_delimiter(str, '=');

        if (results[0] == "headers"){
            headers = stoi(results[1]);
        }

        if (results[0] == "N"){
            N       = stoi(results[1]);
            N_flag = true;
        }

        if (results[0] == "N_s"){
            N_s      = stoi(results[1]);
            N_s_flag = true;
        }

        if (results[0] == "seed_pct"){
            seed_pct           = stod(results[1]);
            seed_pct_flag      = true;
        }

        if (results[0] == "D"){
            D       = stoi(results[1]);
            D_flag  = true;
        }

        if (results[0] == "lattice"){
            lattice      = stoi(results[1]);
            lattice_flag = true;
        }

        if (results[0] == "phi"){
            phi       = stod(results[1]);
            phi_flag  = true;
        }

        if (results[0] == "alpha"){
            alpha      = stod(results[1]);
            alpha_flag = true;
        }

        if (results[0] == "seedMass"){
            seed_mass      = stod(results[1]);
            seed_mass_flag = true;
        }

        if (results[0] == "rng_seed"){
            rng_seed      = stoi(results[1]);
            rng_seed_flag = true;
        }


    }

    parser.close();

    std::vector<std::string> periodic_names;
    std::vector<std::string> L_names;

    L                = (int*)malloc(sizeof(int) * D);
    periodic         = (int*)malloc(sizeof(int) * D);

    for (int axis = 0; axis < D; axis++){
        periodic_names.push_back("x"+std::to_string(axis)+"_periodic");
        L_names.push_back("x"+std::to_string(axis)+"_L");
    }

    parser.open(params_name, std::ifstream::in);
    count = 0;

    while (getline(parser, str)){

        results = split_string_by_delimiter(str, '=');

        for (int axis = 0; axis < D; axis++){

            if (results[0] == periodic_names[axis])
                periodic[axis] = stoi(results[1]);

        }

        for (int axis = 0; axis < D; axis++){

            if (results[0] == L_names[axis])
                L[axis] = stoi(results[1]);

        }
    }

    parser.close();


}

void dlca_lattice_3d::check_params()
{
    double temp;
    int    L_total = 1;

    L_flag = true;

    for (int axis = 0; axis < D; axis++)
        L_total *= L[axis];

    if((N_s_flag == false) && (seed_pct_flag == false)){
        std::cout<<"please provide N_s or seed_pct"<<std::endl;
        exit(EXIT_FAILURE);
    }

    if((phi_flag == false) && (N_flag == false)){
        std::cout<<"please provide N or phi"<<std::endl;
        exit(EXIT_FAILURE);
    }

    if((phi_flag == false) && (L_flag == false)){
        std::cout<<"please provide L or phi"<<std::endl;
        exit(EXIT_FAILURE);
    }

    if((L_flag == false) && (N_flag == false)){
        std::cout<<"please provide L or N"<<std::endl;
        exit(EXIT_FAILURE);
    }

    if ((N_s_flag) && (seed_pct_flag)){
        
        temp = (100. * N_s_flag)/(1. * N);

        if (fabs(temp-seed_pct) > 1e-8){
            std::cout<<"N_s does not match seed_pct"<<std::endl;
            exit(EXIT_FAILURE);
        }

    } 

    if ((N_flag) && (phi_flag) && (L_flag))
    {

        temp = (1.*N)/(1.*L_total);

        if (fabs(temp-phi) > 1e-8){
            std::cout<<"N, L and phi are not consistent"<<std::endl;
            exit(EXIT_FAILURE);
        }

    }

    if (N_s_flag == false){
        N_s = (int)((N*seed_pct)/100);
    }

    if (phi_flag == false){
        phi = (1.*N)/(1.*L_total);
    }

    if (N_flag == false){
        N = (int)(phi*L_total);
    }
    
}

void dlca_lattice_3d::read_config_parser(char *config_filename)
{
    for (int i = 0; i < N; i++){
        
        for (int axis = 0; axis < D; axis++)
            location[axis] = pos(i,axis);

        grid.occupied(location) = N;

    }
        
    std::ifstream parser(config_filename, std::ifstream::in);
    int count = 0;

    std::string str;
    std::vector<std::string> results;

    std::vector<std::string> xi_names;
    std::string assigned_name    = "seed";

    std::vector<int> xi_idx(D);
    int assigned_idx;


    for (int axis = 0; axis < D; axis++)
        xi_names.push_back("x"+std::to_string(axis));

    getline(parser,str);

    results = split_string_by_delimiter(str, ',');

    for (int i = 0; i < results.size(); i++){

        if (results[i] == assigned_name)
            assigned_idx = i;

        for (int axis = 0; axis < D; axis++){
            if (results[i] == xi_names[axis])
                xi_idx[axis] = i;
        }

    }


    count=0;

    while (getline(parser, str)){

        results = split_string_by_delimiter(str, ',');

        /*for (int i = 0; i < results.size(); i++)
            std::cout<<results[i]<<"\t";

        std::cout<<"\n";*/

        seed_[count]  = stoi(results[assigned_idx]);
        
        for (int axis = 0; axis < D; axis++)
            pos(count,axis) = stoi(results[xi_idx[axis]]);

        count++;

    }

    parser.close();

    int L_total = 1;

    for (int axis = 0; axis < D; axis++)
        L_total *= L[axis];

    for (int i = 0; i < N; i++){

        for (int axis = 0; axis < D; axis++)
            location[axis] = pos(i,axis);

        grid.occupied(location) = i;

    }

    for (int i = 0; i < N; i++)
        origSeedStatus_[i] = seed_[i];

    for (int i = 0; i < N; i++)
        mass_[i] = baseMass_[seed_[i]];

    for (int i = 0; i < N; i++){
        checkForAggregations(i);
    }

    reset_check_status();

}