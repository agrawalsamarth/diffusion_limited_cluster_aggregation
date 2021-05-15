#include "post.h"

namespace post_p
{

void postprocessing::get_headers()
{
    headers_ = 1; // for the header itself

    if (lattice_flag_)
        headers_++;

    if (N_flag_)
        headers_++;

    if (D_flag_)
        headers_++;

    if (phi_flag_)
        headers_++;

    if (seed_mass_flag_)
        headers_++;

    if (columns_flag_)
        headers_++;

    if (max_attachments_flag_)
        headers_++;

    if (folded_flag_)
        headers_++;

    if (iters_flag_)
        headers_++;

    if (alpha_flag_)
        headers_++;

    if (L_flag_)
        headers_ += 2 * dim();

    if (periodic_flag_)
        headers_ += dim();

    


}




}