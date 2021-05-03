#include "post.h"

namespace post_p
{
    void postprocessing::dump_delta_coords(int file_number)
    {
        init_unfolding();
        calc_delta_coords();

        int N_eff     = (numParticles()*(numParticles()-1))/2;

        FILE *f;
        char filename[300];

        //sprintf(filename, , file_number);
        //sprintf("./results/delta_coords.csv", "%d.csv", file_number);
        f = fopen("./results/delta_coords.csv", "w");

        for (int i = 0; i < N_eff; i++){
            for (int axis = 0; axis < dim(); axis++){

                if (axis == (dim()-1))
                    fprintf(f,"%lf\n", delta_coords(i,axis));

                else
                    fprintf(f, "%lf,", delta_coords(i,axis));


            }
        }

        fclose(f);

    }

    //void postprocessing::


    void postprocessing::calc_delta_coords()
    {
        int N_eff     = (numParticles()*(numParticles()-1))/2;
        delta_coords_ = (double*)malloc(sizeof(double) * N_eff * dim());

        int count = 0;

        for (int i = 0; i < numParticles(); i++){
            for (int j = i+1; j < numParticles(); j++) {
                
                for (int axis = 0; axis < dim(); axis++)
                    delta_coords(count, axis) = unfolded_coords(j,axis)-unfolded_coords(i,axis);

                count++;


            }
        }


    }


}