#include "post.h"

namespace post_p{

void postprocessing::dump_unfolded_scattering_function()
{
    calc_unfolded_rij();
    calc_unfolded_scattering_function();

    FILE *f;

    f = fopen("./results/s_vs_q.csv", "w");

    for (int i = 0; i < num_q; i++)
        fprintf(f, "%lf,%lf\n", (1.*i*dq)+q_min, sq_unfolded(i));

    fclose(f);

}

void postprocessing::calc_unfolded_scattering_function()
{

    q_min = 0.5;
    q_max = 20.;
    dq    = 0.1;

    num_q = ((q_max-q_min)/dq)+1;
    double q;
    double N_inv = 1./(1. * numParticles());

    int N_eff = (numParticles() * (numParticles() - 1))/2;

    sq_unfolded_ = (double*)malloc(sizeof(double)*num_q);

    for (int i = 0; i < num_q; i++)
        sq_unfolded(i) = 1.;

    for (int i = 0; i < num_q; i++){

        q = (1.*i*dq)+q_min;

        for (int j = 0; j < N_eff; j++){
            sq_unfolded(i) += (N_inv * ((sin(q*r_ij_[j]))/(q*r_ij_[j])));
        }

    }

}

}