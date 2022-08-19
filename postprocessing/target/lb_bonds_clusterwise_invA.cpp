#include <post.h>

int main(int argc, char *argv[])
{
    post_p::postprocessing *test = new post_p::postprocessing(argv[1]);

    if (argc < 3){
        std::cout<<"please provide config_filename results_filename"<<std::endl;
        return 0;
    }

    test->determine_LB_bonds_clusterwise(argv[2]);

    return 0;
}