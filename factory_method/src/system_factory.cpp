#include <system_factory.hh>

namespace simulation{

constituent* system_factory::create_constituent(std::string name_type, int lattice, int dim, simulation_box *box){

    if ((lattice == 1) && (strcmp(name_type,"particle")==0))
        return new particle<int>(dim, box);

    else if ((lattice == 0) && (strcmp(name_type,"particle")==0))
        return new particle<double>(dim, box);

    else if ((lattice == 1) && (strcmp(name_type,"cluster")==0))
        return new cluster<int>;

    else if ((lattice == 0) && (strcmp(name_type,"cluster")==0))
        return new cluster<double>;

    else{
        std::cout<<"unknown constituent"<<std::endl;
        exit(EXIT_FAILURE);
    }
}

simulation_box* system_factory::create_simulation_box(int lattice, int dim, int *box_lengths, vector<boundary_conditions*> system_bc){

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

    if (strcmp(name_type, "periodic"))
        return new periodic_bc;
    else{
        std::cout<<"unknown boundary condition"<<std::endl;
        exit(EXIT_FAILURE);
    }

}


}