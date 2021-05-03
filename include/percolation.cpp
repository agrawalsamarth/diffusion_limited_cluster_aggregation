#include "post.h"

namespace post_p
{
    bool postprocessing::check_percolation()
    {
        bool percolation_test = false;

        for (int i = 0; i < numParticles(); i++){
            if (numAttachments(i) != unfoldedAttachments(i)){
                percolation_test = true;
            }
        }

        return percolation_test;

    }

}