#include "post.h"

namespace post_p
{

    void postprocessing::memory_allocation_function()
    {


        lo_hi_           = (double*)malloc(sizeof(double) * dim() * 2);
        L_               = (double*)malloc(sizeof(double) * dim());
        halfL_           = (double*)malloc(sizeof(double) * dim());
        periodic_        = (int*)malloc(sizeof(int) * dim());
        pos_             = (double*)malloc(sizeof(double) * numParticles() * dim());
        num_attachments_ = (int*)malloc(sizeof(int) * numParticles());
        original_seed_   = (int*)malloc(sizeof(int) * numParticles());
        current_seed_    = (int*)malloc(sizeof(int) * numParticles());
        attachment_      = (int*)malloc(sizeof(int) * numParticles() * maxAttachments());
        diameter_        = (double*)malloc(sizeof(double) * numParticles());

        posDiff_         = (double*)malloc(sizeof(double) * dim());

        is_placed_                = (bool*)malloc(sizeof(bool) * numParticles());
        unfolded_coords_          = (double*)malloc(sizeof(double) * numParticles() * dim());
        unfolded_num_attachments_ = (int*)malloc(sizeof(int) * numParticles());
        cluster_percolation_      = (int*)malloc(sizeof(int) * numParticles() * dim());



    }


}