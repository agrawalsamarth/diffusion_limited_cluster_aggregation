#include "post.h"

namespace post_p{

void postprocessing::dump_unfolded_hist_file(double bin_size)
{
    //std::cout<<"1"<<std::endl;
    calc_unfolded_rij();
    //std::cout<<"2"<<std::endl;
    calc_unfolded_hist(bin_size);
    //std::cout<<"3"<<std::endl;

    double r_max = halfL(0);
    int num_bins = r_max/bin_size;
    int N_eff    = (numParticles() * (numParticles() - 1))/2;
    int bin;

    //std::cout<<"4"<<std::endl;

    FILE *f;

    f = fopen("../results/unfolded_hist.csv", "w");

    for (int i = 0; i < num_bins; i++)
        fprintf(f, "%lf,%lf\n", (1.*i*bin_size + 0.5 * bin_size), hist_[i]/(1. * N_eff));

    fclose(f);

        //std::cout<<"5"<<std::endl;

}

void postprocessing::calc_unfolded_rij()
{
    init_unfolding();

    int N_eff = (numParticles() * (numParticles() - 1))/2;
    //r_ij_     = (double*)malloc(sizeof(double) * N_eff);
    r_ij_ = new double[N_eff];

    int counter = 0;
    double r_temp;

    for (int i = 0; i < numParticles(); i++) {
        for (int j = i+1; j < numParticles(); j++){

            for (int axis = 0; axis < dim(); axis++)
                posDiff(axis) = 0.;

            for (int axis = 0; axis < dim(); axis++){
                posDiff(axis) += unfolded_coords(j,axis)-unfolded_coords(i,axis);
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



void postprocessing::calc_unfolded_hist(double bin_size)
{

    //std::cout<<"1"<<std::endl;
    double r_max = halfL(0);
    //std::cout<<"2"<<std::endl;
    int num_bins = r_max/bin_size;
    //std::cout<<"3"<<std::endl;
    int bin;
    //std::cout<<"4"<<std::endl;

    //hist_ = (double*)malloc(sizeof(double)*num_bins);
    hist_ = new double[num_bins];
    //std::cout<<"5"<<std::endl;

    for (int i = 0; i < num_bins; i++)
        hist_[i] = 0.;

    //std::cout<<"6"<<std::endl;

    int N_eff    = (numParticles() * (numParticles() - 1))/2;

    //std::cout<<"7"<<std::endl;

    for (int i = 0; i < N_eff; i++){

        if (r_ij_[i] < r_max){
            bin         = r_ij_[i]/bin_size;
            hist_[bin] += 1.;
        }


    }

    //std::cout<<"8"<<std::endl;

    for (int i = 1; i < num_bins; i++)
        hist_[i] += hist_[i-1];


}


}

