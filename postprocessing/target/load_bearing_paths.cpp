#include <post.h>

int main(int argc, char *argv[])
{
    post_p::postprocessing test(argv[1]);

    test.check_percolation();

    if (argc < 3)
        test.dump_load_bearing_paths_file();

    else
        test.dump_load_bearing_paths_file(argv[2]);

    return 0;
}