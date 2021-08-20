#include <dlma_iterator.hh>

using namespace simulation;

int main(int argc, char *argv[])
{
    if (argc < 2){
        std::cout<<"please provide params file"<<std::endl;
        exit(EXIT_FAILURE);
    }


    system_iterator<int> *test = new dlma_iterator<int>(argv[1]);
    test->run_system();

    if (argc < 3)
        test->save_config_file();
    else
        test->save_config_file(argv[2]);

    return 0;

}