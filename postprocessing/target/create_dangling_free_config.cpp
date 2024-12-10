#include <post.h>
#include <ctime>

int main(int argc, char *argv[])
{
    post_p::postprocessing *test = new post_p::postprocessing(argv[1]);
    
    if (argc < 3){
        std::cout<<"please provide config_filename results_filename"<<std::endl;
        return 0;
    }

    else{
        test->dump_dangling_free_config(argv[2]);
    }



    return 0;
}