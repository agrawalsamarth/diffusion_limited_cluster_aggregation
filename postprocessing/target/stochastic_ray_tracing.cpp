#include <post.h>

int main(int argc, char *argv[])
{

    if (argc < 4){
        std::cout<<"check number of arguments"<<std::endl;
        return 0;
    }

    post_p::postprocessing *test = new post_p::postprocessing(argv[1]);
    test->dump_stochastic_ray_tracing(argv[2], std::stoi(argv[3]));

    return 0;

}