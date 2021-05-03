#include "post.h"

namespace post_p{

postprocessing::postprocessing()
{
    read_params();
    read_config();
}

postprocessing::~postprocessing()
{
    free(L_);
    free(halfL_);
    free(periodic_);
    free(pos_);
    free(num_attachments_);
    free(attachment_);
    if (r_ij_flag_)
        free(r_ij_);
    if (hist_flag_)
        free(hist_);
    if (is_placed_flag_)
        free(is_placed_);
    if (unfolded_coords_flag_)
        free(unfolded_coords_);
}

}
