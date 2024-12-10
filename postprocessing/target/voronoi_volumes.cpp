#include <post.h>

int main(int argc, char *argv[])
{
    post_p::postprocessing *test = new post_p::postprocessing(argv[1]);
    
    if (argc < 3){
        std::cout<<"please provide config_filename results_filename"<<std::endl;
        return 0;
    }

    else{
        test->dump_voronoi_volume(argv[2]);
    }

    delete test;
    return 0;
}