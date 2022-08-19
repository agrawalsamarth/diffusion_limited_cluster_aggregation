#include <post.h>
#include <ctime>

int main(int argc, char *argv[])
{
    post_p::postprocessing *test = new post_p::postprocessing(argv[1]);
    FILE *f_time;
    clock_t begin;
    clock_t end;

    if (argc < 3){
        std::cout<<"please provide config_filename results_filename"<<std::endl;
        return 0;
    }

    else if (argc == 3)
        test->determine_LB_bonds_clusterwise(argv[2]);

    else{

        begin = clock();
        test->determine_LB_bonds_clusterwise(argv[2]);
        end = clock();

        f_time = fopen(argv[3],"w");
        fprintf(f_time, "cpu_time\n");
        fprintf(f_time, "%lf\n", double(end - begin)/CLOCKS_PER_SEC);
        fclose(f_time);

    }

    return 0;
}