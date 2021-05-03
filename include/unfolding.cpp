#include "post.h"

namespace post_p
{

void postprocessing::dump_unfolded_file()
{
    init_unfolding();
    bool percolation_test = check_percolation();

    if (percolation_test)
        std::cout<<"cluster is percolating, can't unfold"<<std::endl;

    else {
        FILE *f;
        char filename[300];

        f = fopen("./results/unfolded_coords.csv", "w");

        for (int i = 0; i < numParticles(); i++){
            for (int axis = 0; axis < dim(); axis++){

                if (axis == (dim()-1)){
                    fprintf(f, "%lf\n", unfolded_coords(i,axis));
                }
            
                else{
                    fprintf(f, "%lf,", unfolded_coords(i,axis));
                }
            }
        }

        fclose(f);
    }


}

void postprocessing::init_unfolding()
{

    is_placed_       = (bool*)malloc(sizeof(bool) * numParticles());
    unfolded_coords_ = (double*)malloc(sizeof(double) * numParticles() * dim());
    unfolded_num_attachments_ = (int*)malloc(sizeof(int) * numParticles());

    is_placed_flag_       = true;
    unfolded_coords_flag_ = true;

    for (int i = 0; i < numParticles(); i++) {
        is_placed(i) = false;
        unfoldedAttachments(i) = 0;
    }

    for (int axis = 0; axis < dim(); axis++)
        unfolded_coords(0,axis) = 0.;

    unfold(0,0);

}

void postprocessing::unfold(const int prev, const int next)
{

    for (int axis = 0; axis < dim(); axis++)
        posDiff(axis) = get_periodic_image(pos(next,axis) - pos(prev,axis), axis);

    for (int axis = 0; axis < dim(); axis++)
        unfolded_coords(next,axis) = unfolded_coords(prev,axis) + posDiff(axis);

    is_placed(next) = true;

    for (int att = 0; att < numAttachments(next); att++)
    {
        temp_next = attachment(next, att);
        
        if (is_placed(temp_next) == false){
            unfoldedAttachments(next)      = unfoldedAttachments(next) + 1;
            unfoldedAttachments(temp_next) = unfoldedAttachments(temp_next) + 1;
            unfold(next, temp_next);
        }

    }


}


}