#include <dlma_system.hh>

namespace simulation{

template<typename type>
std::vector<std::string> dlma_system<type>::split_string_by_delimiter(const std::string& s, char delimiter)
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


template<typename type>
void dlma_system<type>::read_params_parser(char *params_name)
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

    L                = (type*)malloc(sizeof(int) * D);
    L_flag           = (bool*)malloc(sizeof(bool) * D);
    //periodic         = (int*)malloc(sizeof(int) * D);
    
    std::vector<boundary_conditions<type>*> system_bc(D);
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
                system_bc[axis] = factory.create_boundary_conditions(results[1]);
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

    type L_total = 1;

    if (L_flag_and == false){

        for (int axis = 0; axis < D; axis++)
            L[axis] = (type)(pow((1.*N)/(1.*phi), 1./(1.*D)));
        

        for (int axis = 0; axis < D; axis++)
            L_total *= L[axis];

        phi = (1.*N)/(1*L_total);

    }

    double temp;
    L_total = 1;

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


    box = factory.create_simulation_box(lattice, D, L, system_bc);

    generator.seed(rng_seed);
    dis.param(std::uniform_real_distribution<double>::param_type(0.0, 1.0));
    attachments.resize(N);

}

template<typename type>
void dlma_system<type>::calculate_propensity()
{

    int agg_size = aggregates.size();

    propensity     = (double*)malloc(sizeof(double) * agg_size);
    cum_propensity = (double*)malloc(sizeof(double) * agg_size);

    for (int i = 0; i < agg_size; i++){
        propensity[i]     = 0.;
        cum_propensity[i] = 0.;
    }

    for (int i = 0; i < agg_size; i++){
        propensity[i] = std::pow(aggregates[i]->get_mass(), alpha);
    }

}

template<typename type>
void dlma_system<type>::add_aggregate(constituent<type> *new_aggregate){
    aggregates.push_back(new_aggregate);
}

template<typename type>
void dlma_system<type>::remove_aggregate(const int id){

    for (int i = 0; i < aggregates.size(); i++){

        if (aggregates[i]->get_id() == id)
            aggregates.erase(aggregates.begin()+i);

    }

}

template<typename type>
constituent<type>* dlma_system<type>::get_aggregate(const int id)
{

    for (int i = 0; i < aggregates.size(); i++){

        if (aggregates[i]->get_id() == id)
            return aggregates[i];

    }

    return NULL;


}

template<typename type>
void dlma_system<type>::build_id_map()
{

    for (int i = 0; i < N; i++)
        id_map[all_particles[i]->get_id()] = all_particles[i]->get_aggregate_id();

}

template<typename type>
int dlma_system<type>::get_latest_cluster_id(){

    return latest_cluster_id++;

}

template<typename type>
int dlma_system<type>::get_latest_cluster_id_without_increment(){
    return latest_cluster_id;
}

template<typename type>        
int dlma_system<type>::get_id_map(int c_id){

    return id_map[c_id];

}

template<typename type>
int dlma_system<type>::get_lattice()
{ return lattice; }

template<typename type>
int dlma_system<type>::get_dim()
{ return D; }

template<typename type>
int dlma_system<type>::get_max_attachments()
{
    int max_atts = 0;

    for (int i = 0; i < N; i++){

        if (attachments[i].size() > max_atts)
            max_atts = attachments[i].size();

    }

    return max_atts;
}

template<typename type>
simulation_box<type>* dlma_system<type>::get_box()
{ return box; }

template<typename type>
constituent<type>* dlma_system<type>::get_constituent(const int i){
    return aggregates[i];
}

template<typename type>
void dlma_system<type>::print_id_map(){

    for (const auto& x : id_map) {
            std::cout << x.first << ": " << x.second << "\n";
    }

}

template<typename type>
int dlma_system<type>::total_aggregates()
{ return aggregates.size();}

template<typename type>
void dlma_system<type>::add_attachment(constituent<type> *c_1)
{

    int particle_id;
    int neighbour_id;

    std::vector<int> neighbours;

    for (int i = 0; i < c_1->get_size(); i++){

        particle_id = c_1->get_element_id(i);
        neighbours  = c_1->get_neighbour_list(i);

        for (int j = 0; j < neighbours.size(); j++) {

            neighbour_id = neighbours[j];

            if (neighbour_id != -1){

                if(!(std::find(attachments[particle_id].begin(), attachments[particle_id].end(), neighbour_id) != attachments[particle_id].end())){

                    attachments[particle_id].push_back(neighbour_id);
                    attachments[neighbour_id].push_back(particle_id);

                } 


            }


        }

    }
}

template<typename type>
void dlma_system<type>::print_attachments(){

    std::cout<<"attachments size = "<<attachments.size()<<std::endl;

    for (int i = 0; i < N; i++){

        std::cout<<i<<"\t";

        for (int j = 0; j < attachments[i].size(); j++)
            std::cout<<attachments[i][j]<<"\t";

        std::cout<<"\n";

    }


}

template<typename type>
double dlma_system<type>::get_seedmass()
{ return seed_mass;}

template<typename type>
constituent<type>* dlma_system<type>::get_particle_by_id(const int id)
{

    for (int i = 0; i < N; i++){
        if (all_particles[i]->get_id() == id)
            return all_particles[i];
    }

    return NULL;

}

template<typename type>
int dlma_system<type>::get_N()
{ return N;}

template<typename type>
double dlma_system<type>::get_phi()
{ return phi; }

template<typename type>
double dlma_system<type>::get_alpha()
{ return alpha; }

template<typename type>
std::vector<int> dlma_system<type>::get_attachment_vector(const int i)
{return attachments[i];}

template class dlma_system<int>;
template class dlma_system<double>;


}