#include <dlca_lattice_3d.h>

int main(int argc, char *argv[])
{
    //int file_number = atoi(argv[1]);
    //std::cout<<"start"<<std::endl;

    dlca_lattice_3d test(argv[1]);
    test.runSimulation();
    test.save_config();
    return 0;
}
