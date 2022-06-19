#include <post.h>

namespace post_p
{

void postprocessing::dump_unfolded_file()
{
    bool percolation_test;

    if (max_attachments_ > 0){
        percolation_test = check_percolation();

        if (percolation_test){
            dump_percolation_file();
        }

        else {
            save_unfolded_config();
        }
    }

    else{

        for (int i = 0; i < numParticles(); i++){
            for (int axis = 0; axis < dim(); axis++){
                unfolded_coords(i,axis) = pos(i,axis);
            }
        }

        save_unfolded_config();


    }

}

void postprocessing::dump_unfolded_file(char *filename)
{
    bool percolation_test;

    if (max_attachments_ > 0){
        percolation_test = check_percolation();

        if (percolation_test){
            dump_percolation_file(filename);
        }

        else {
            save_unfolded_config(filename);
        }
    }

    else{

        for (int i = 0; i < numParticles(); i++){
            for (int axis = 0; axis < dim(); axis++){
                unfolded_coords(i,axis) = pos(i,axis);
            }
        }

        save_unfolded_config(filename);


    }

}

void postprocessing::init_unfolding()
{

    for (int i = 0; i < numParticles(); i++){
        for (int axis = 0; axis < dim(); axis++){
            cluster_percolation(i,axis)=0;
        }
    }

    totalClusters_ = 0;

    for (int i = 0; i < numParticles(); i++) {
        is_placed(i) = false;
        unfoldedAttachments(i) = 0;
        for (int axis = 0; axis < dim(); axis++)
            unfolded_coords(i,axis) = 0.;
    }

    for (int i = 0; i < numParticles(); i++){
        if (is_placed(i) == false){
            unfold(i,i);
            totalClusters_+=1;
        }
    }

}

void postprocessing::init_unfolding_for_lbp()
{
    for (int i = 0; i < numParticles(); i++){
        for (int axis = 0; axis < dim(); axis++){
            cluster_percolation(i,axis)=0;
        }
    }

    totalClusters_ = 0;

    for (int i = 0; i < numParticles(); i++) {

        for (int j = 0; j < numParticles(); j++){
            is_placed(j) = false;
            unfoldedAttachments(j) = 0;
            for (int axis = 0; axis < dim(); axis++)
                unfolded_coords(j,axis) = 0.;
        }

        for (int axis = 0; axis < dim(); axis++)
            temp_lbp_[axis] = 0;

        unfold_for_lbp(i,i);

        for (int axis = 0; axis < dim(); axis++)
            load_bearing_paths(i,axis) = temp_lbp_[axis];

    }


}

void postprocessing::unfold(const int prev, const int next)
{

    for (int axis = 0; axis < dim(); axis++)
        posDiff(axis) = get_periodic_image(pos(next,axis) - pos(prev,axis), axis);

    for (int axis = 0; axis < dim(); axis++){
        if (prev == next)
            unfolded_coords(next,axis) = pos(prev,axis) + posDiff(axis);
        else
            unfolded_coords(next,axis) = unfolded_coords(prev,axis) + posDiff(axis);
    }

    is_placed(next) = true;

    for (int att = 0; att < numAttachments(next); att++)
    {
        temp_next = attachment(next, att);
        
        if (is_placed(temp_next) == false){
            unfoldedAttachments(next)      = unfoldedAttachments(next) + 1;
            unfoldedAttachments(temp_next) = unfoldedAttachments(temp_next) + 1;
            unfold(next, temp_next);
        }

        else {
            
            for (int axis = 0; axis < dim(); axis++) {
                posDiff(axis) = unfolded_coords(temp_next, axis) - unfolded_coords(next, axis);
                if ((posDiff(axis) > halfL(axis)) || (posDiff(axis) < -halfL(axis))){
                    cluster_percolation(totalClusters_,axis) = 1;
                    load_bearing_paths_[axis]++;
                }
            }

        }

    }


}

void postprocessing::unfold_for_lbp(const int prev, const int next)
{

    for (int axis = 0; axis < dim(); axis++)
        posDiff(axis) = get_periodic_image(pos(next,axis) - pos(prev,axis), axis);

    for (int axis = 0; axis < dim(); axis++){
        if (prev == next)
            unfolded_coords(next,axis) = pos(prev,axis) + posDiff(axis);
        else
            unfolded_coords(next,axis) = unfolded_coords(prev,axis) + posDiff(axis);
    }

    is_placed(next) = true;

    for (int att = 0; att < numAttachments(next); att++)
    {
        temp_next = attachment(next, att);
        
        if (is_placed(temp_next) == false){
            unfoldedAttachments(next)      = unfoldedAttachments(next) + 1;
            unfoldedAttachments(temp_next) = unfoldedAttachments(temp_next) + 1;
            unfold_for_lbp(next, temp_next);
        }

        else {
            
            for (int axis = 0; axis < dim(); axis++) {
                posDiff(axis) = unfolded_coords(temp_next, axis) - unfolded_coords(next, axis);
                if ((posDiff(axis) > halfL(axis)) || (posDiff(axis) < -halfL(axis))){
                    cluster_percolation(totalClusters_,axis) = 1;
                    temp_lbp_[axis]++;
                }
            }

        }

    }

}



}