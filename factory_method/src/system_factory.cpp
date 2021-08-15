#include <system_factory.hh>

namespace simulation{

constituent<int>* system_factory::create_constituent(int constituent_id, int lattice, int dim, std::string name_type, simulation_box *box){

    if ((lattice == 1) && (strcmp(name_type.c_str(),"particle")==0))
        return new particle<int>(constituent_id, dim, box);

    //else if ((lattice == 0) && (strcmp(name_type,"particle")==0))
        //return new particle<double>(dim, box);

    else if ((lattice == 1) && (strcmp(name_type.c_str(),"cluster")==0))
        return new cluster<int>(constituent_id, dim, box);

    //else if ((lattice == 0) && (strcmp(name_type,"cluster")==0))
        //return new cluster<double>;

    else{
        std::cout<<"unknown constituent"<<std::endl;
        exit(EXIT_FAILURE);
    }
}

simulation_box* system_factory::create_simulation_box(int lattice, int dim, int *box_lengths, std::vector<boundary_conditions*> system_bc){

    if (lattice == 1){
        return new on_lattice(dim, box_lengths, system_bc);
    }

    else{
        std::cout<<"unknown simulation box"<<std::endl;
        exit(EXIT_FAILURE);
    }

}

/*simulation_box* system_factory::create_simulation_box(int lattice, int dim, double *box_lengths){

    if (lattice == 1){
        return new on_lattice(dim, box_lengths);
    }

    else{
        std::cout<<"unknown box type"<<std::endl;
        exit(EXIT_FAILURE);
    }

}*/


boundary_conditions* system_factory::create_boundary_conditions(std::string name_type){

    if (strcmp(name_type.c_str(), "periodic"))
        return new periodic_bc;
    else{
        std::cout<<"unknown boundary condition"<<std::endl;
        exit(EXIT_FAILURE);
    }

}


}