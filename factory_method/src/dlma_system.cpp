#include <dlma_system.hh>

namespace simulation{

std::vector<std::string> dlma_system::split_string_by_delimiter(const std::string& s, char delimiter)
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

dlma_system::dlma_system(char *params_name)
{

    read_params_parser(params_name);
    check_params();
    initialize_system();

}

void dlma_system::read_params_parser(char *params_name)
{

    std::ifstream parser(params_name, std::ifstream::in);

    if (parser.fail()){
        std::cout<<"either file does not exist or does not have permissions"<<std::endl;
        exit(EXIT_FAILURE);
    }

    int count = 0;

    std::string str;
    std::vector<std::string> results;

    while (getline(parser, str)){

        results = split_string_by_delimiter(str, '=');

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

    if (D_flag == false){
        std::cout<<"please provide dimensions value"<<std::endl;
        exit(EXIT_FAILURE);
    }

    if (lattice_flag == false){
        std::cout<<"please provide lattice value"<<std::endl;
        exit(EXIT_FAILURE);
    }

    std::vector<std::string> bc_names;
    std::vector<std::string> L_names;

    L                = (int*)malloc(sizeof(int) * D);
    L_flag           = (bool*)malloc(sizeof(bool) * D);
    //periodic         = (int*)malloc(sizeof(int) * D);
    
    std::vector<boundary_conditions*> system_bc(D);
    periodic_flag    = (bool*)malloc(sizeof(bool) * D);


    for (int axis = 0; axis < D; axis++){
        bc_names.push_back("x"+std::to_string(axis)+"_bc");
        L_names.push_back("x"+std::to_string(axis)+"_L");
    }

    for (int axis = 0; axis < D; axis++){
        L_flag[axis] = false;
        periodic_flag[axis] = false;
    }

    L_flag_and = true;
    L_flag_or  = false;
    periodic_flag_and = true;

    parser.open(params_name, std::ifstream::in);
    count = 0;

    while (getline(parser, str)){

        results = split_string_by_delimiter(str, '=');

        for (int axis = 0; axis < D; axis++){

            if (results[0] == bc_names[axis]){
                system_bc[axis] = factory.create_boundary_condition(bc_names[axis]);
                periodic_flag[axis] = true;
            }

        }

        for (int axis = 0; axis < D; axis++){

            if (results[0] == L_names[axis]){
                L[axis] = stoi(results[1]);
                L_flag[axis] = true;
            }

        }
    }

    parser.close();

    for (int axis = 0; axis < D; axis++){
        L_flag_and = (L_flag_and && L_flag[axis]);
        L_flag_or  = (L_flag_or  || L_flag[axis]);

        periodic_flag_and = (periodic_flag_and && periodic_flag[axis]);
    }

    if (periodic_flag_and == false){
        std::cout<<"please provide boundary conditions in each direction"<<std::endl;
        exit(EXIT_FAILURE);
    }

}

void dlma_system::check_params()
{
    double temp;
    int    L_total = 1;

    if ((N_flag) && (N_s_flag)){

        if (N_s > N){
            std::cout<<"number of seeds is greater than number of particles"<<std::endl;
            exit(EXIT_FAILURE);
        }

    }

    if (phi_flag){

        if (phi > 1){
            std::cout<<"phi should be <= 1"<<std::endl;
            exit(EXIT_FAILURE);
        }

    }

    if (seed_pct_flag){

        if (seed_pct > 100){
            std::cout<<"seed_pct should be <= 100"<<std::endl;
            exit(EXIT_FAILURE);
        }

    }

    if (L_flag_and != L_flag_or){
        std::cout<<"length in one direction is missing"<<std::endl;
        exit(EXIT_FAILURE);
    }

    if((N_s_flag == false) && (seed_pct_flag == false)){
        std::cout<<"please provide N_s or seed_pct"<<std::endl;
        exit(EXIT_FAILURE);
    }

    if((phi_flag == false) && (N_flag == false)){
        std::cout<<"please provide N or phi"<<std::endl;
        exit(EXIT_FAILURE);
    }

    if((phi_flag == false) && (L_flag_and == false)){
        std::cout<<"please provide L or phi"<<std::endl;
        exit(EXIT_FAILURE);
    }

    if((L_flag_and == false) && (N_flag == false)){
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

    if (L_flag_and){
        for (int axis = 0; axis < D; axis++)
            L_total *= L[axis];
    }

    if ((N_flag) && (phi_flag) && (L_flag_and))
    {

        temp = (1.*N)/(1.*L_total);

        if (fabs(temp-phi) > 1e-8){
            std::cout<<"N, L and phi are not consistent"<<std::endl;
            exit(EXIT_FAILURE);
        }

    }

    if (phi_flag == false){
        phi = (1.*N)/(1.*L_total);
    }

    if (N_flag == false){
        N = (int)(phi*L_total);
    }

    if (N_s_flag == false){
        N_s = (int)((N*seed_pct)/100);
    }

    if (L_flag_and == false){

        for (int axis = 0; axis < D; axis++)
            L[axis] = (int)(pow((1.*N)/(1.*phi), 1./(1.*D)));
        

        for (int axis = 0; axis < D; axis++)
            L_total *= L[axis];

        phi = (1.*N)/(1*L_total);

    }
    
}

void dlma_system::initialize_system()
{

    box = factory.create_simulation_box(lattice, D, L);

    std::cout<<"done"<<std::endl;

}


}