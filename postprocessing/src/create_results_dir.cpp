#include <post.h>

namespace post_p{

void postprocessing::create_results_dir()
{
    char parent_folder[] = "./postprocessing_results/";
    create_dir(parent_folder);
    create_dir(folder_name_);

}


}