#include <post.h>

int main(int argc, char *argv[])
{
    post_p::postprocessing test(argv[1]);
    test.dump_rog();
    return 0;
}