#include <post.h>

int main(int argc, char *argv[])
{
    post_p::postprocessing test(argv[1]);
    int total_iters = std::stoi(argv[2]);
    test.psd(total_iters);

    if (argc < 4)
        test.save_radius_distribution();
    else
        test.save_radius_distribution(argv[3]);
    
    return 0;
}