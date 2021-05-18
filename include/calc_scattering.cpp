#include "post.h"

namespace post_p{

void postprocessing::dump_scattering_function(double q_min, double q_max, int num_q)
{
    calc_scattering_function(q_min, q_max, num_q);
    create_results_dir();

    double dq = log(q_max/q_min)/(1 * num_q);


    FILE *f;
    char filename[] = "sq_vs_q.csv";  
    create_filepath(folder_name_, filename);
    f= fopen(filepath_,"w");

    fprintf(f, "q,S(q)\n");

    for (int i = 0; i < num_q; i++)
        fprintf(f, "%lf,%lf\n", exp((1.*i*dq)+log(q_min)), sq_[i]);

    fclose(f);
    free(sq_);

}

void postprocessing::calc_scattering_function(double q_min, double q_max, int num_q)
{
    calc_rij();

    double N_inv = 1./(1.* numParticles());
    double q;
    double dq = log(q_max/q_min)/(1 * num_q);

    sq_ = (double*)malloc(sizeof(double)*num_q);

    for (int i = 0; i < num_q; i++)
        sq_[i] = 1.;

    for (int i = 0; i < num_q; i++){

        q = exp((1.*i*dq)+log(q_min));

        for (int j = 0; j < N_pairs_; j++){
            sq_[i] += (N_inv * ((sin(q*r_ij_[j]))/(q*r_ij_[j])));
        }

    }

}

}