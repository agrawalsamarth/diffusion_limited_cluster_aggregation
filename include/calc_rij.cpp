#include "post.h"

namespace post_p{

void postprocessing::dump_rij_file()
{
    calc_rij();
    FILE *f;
    char filename[] = "r_ij.csv";  
    create_filepath(folder_name_, filename);
    f= fopen(filepath_,"w");
    
    fprintf(f, "r_ij\n");

    for (int i = 0; i < N_pairs_; i++)
        fprintf(f, "%lf\n", r_ij_[i]);
    
    fclose(f);

    
}

void postprocessing::dump_rij_hist_file(double bin_size)
{
    calc_rij_hist(bin_size);
    create_results_dir();

    FILE *f;
    char filename[] = "cumulative_pair_correlation_hist.csv";  
    create_filepath(folder_name_, filename);
    f= fopen(filepath_,"w");

    fprintf(f, "r_ij,P(r)\n");

    for (int i = 0; i < r_ij_hist_bins_; i++){
        fprintf(f, "%lf,%lf\n", (1.*i*bin_size + 0.5 * bin_size), r_ij_hist_[i]/(1. * N_pairs_));
    }

    fclose(f);
    free(r_ij_hist_);

}

void postprocessing::calc_rij()
{
    int counter = 0;
    double r_temp;

    for (int i = 0; i < numParticles(); i++) {
        for (int j = i+1; j < numParticles(); j++){

            for (int axis = 0; axis < dim(); axis++)
                posDiff(axis) = 0.;

            for (int axis = 0; axis < dim(); axis++){
                posDiff(axis) += get_periodic_image(1.*(pos(i,axis) - pos(j,axis)), axis);
            }

            r_temp = 0.;

            for (int axis = 0; axis < dim(); axis++)
                r_temp += (posDiff(axis) * posDiff(axis));

            r_temp = sqrt(r_temp);

            r_ij_[counter] = r_temp;
            counter++;

        }
    }

}

void postprocessing::calc_rij_hist(double bin_size)
{
    calc_rij();
    double r_max    = halfL(0);
    r_ij_hist_bins_ = (((int)(r_max/bin_size))+1);
    r_ij_hist_      = (double*)malloc(sizeof(double)*r_ij_hist_bins_);

    for (int i = 0; i < r_ij_hist_bins_; i++)
        r_ij_hist_[i] = 0.;

    int bin;

    for (int i = 0; i < N_pairs_; i++){

        if (r_ij_[i] < r_max){
            bin              = r_ij_[i]/bin_size;
            r_ij_hist_[bin] += 1.;
        }


    }

    for (int i = 1; i < r_ij_hist_bins_; i++)
        r_ij_hist_[i] += r_ij_hist_[i-1];


}


}