#include "post.h"

namespace post_p{

postprocessing::postprocessing(char *user_folder)
{
    parsed_folder_ = (char*)malloc(strlen("./parsed_file/")+strlen(user_folder)+strlen("/")+1);
    strcpy(parsed_folder_,"./parsed_file/");
    strcat(parsed_folder_,user_folder);
    strcat(parsed_folder_,"/");

    read_params();
    memory_allocation_function();
    read_config();

    char parent_folder[] = "./postprocessing_results/";
    folder_name_ = (char*)malloc(strlen(parent_folder)+strlen(user_folder)+strlen("/")+1);
    strcpy(folder_name_,parent_folder);
    strcat(folder_name_,user_folder);
    strcat(folder_name_,"/");

}


postprocessing::~postprocessing()
{
        free(parsed_folder_);
        free(folder_name_);

        free(lo_hi_);
        free(L_);
        free(halfL_);
        free(periodic_);
        free(pos_);
        free(num_attachments_);
        free(original_seed_);
        free(current_seed_);
        if (max_attachments_ > 0)
            free(attachment_);
        free(diameter_);

        free(posDiff_);

        free(is_placed_);
        free(unfolded_coords_);
        free(unfolded_num_attachments_);
        free(cluster_percolation_);

}

}
