#include <dlca_lattice_3d.h>

int main(int argc, char *argv[])
{
    //int file_number = atoi(argv[1]);
    //std::cout<<"start"<<std::endl;

    dlca_lattice_3d test(argv[1]);
    test.read_config_parser(argv[2]);
    test.force_moves(argv[3]);
    test.save_config();

    return 0;
}