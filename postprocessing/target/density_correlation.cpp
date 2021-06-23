#include <post.h>

int main(int argc, char *argv[])
{
    post_p::postprocessing test(argv[1]);

    if (argc < 3)
        test.dump_density_correlation();

    else
        test.dump_density_correlation(argv[2]);

    return 0;
}