#include <dlca_lattice_3d.h>

int main(int argc, char *argv[])
{   
    if (argc < 2){
        std::cout<<"please provide params file"<<std::endl;
        return 0;
    }

    dlca_lattice_3d test(argv[1]);

    int skip_iters = atoi(argv[2]);
    int iters=0;

    std::string prefix(argv[3]);
    std::string filename;

    while ((test.totalWalkers() != 0) || (test.totalClusters() != 1)) {

        test.moveCluster();

        if (iters % skip_iters == 0){

            filename = prefix+std::to_string(iters);
	        filename = filename+".xyz";
            test.dump_xyz_file(filename.c_str());

        }

        iters++;

    }

    if (iters % skip_iters != 0){

        filename = prefix+std::to_string(iters);
	    filename = filename+".xyz";
        test.dump_xyz_file(filename.c_str());

    }

        
    return 0;
}
