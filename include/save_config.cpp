#include "post.h"

namespace post_p
{

void postprocessing::save_config()
{
    FILE *f;
    int num_atts;
    char filename[] = "config.csv";
  
    create_filepath(folder_name_, filename);
    f= fopen(filepath_,"w");

    get_headers();

    fprintf(f, "headers=%d\n", headers_);
    
    if (lattice_flag_){                         
        fprintf(f, "lattice=%d\n",lattice_);
    }

    if (N_flag_){
        fprintf(f, "N=%d\n", numParticles());
    }

    if (D_flag_){
        fprintf(f, "D=%d\n", dim());
    }

    if (max_attachments_flag_){
        fprintf(f, "maxAttachments=%d\n", maxAttachments());
    }

    if (folded_flag_)
        fprintf(f, "folded=%d\n", folded_);

    if (phi_flag_)
        fprintf(f, "phi=%lf\n", phi_);


    if (iters_flag_)
    {
        fprintf(f, "iters=%d\n", iters_);
    }

    if (alpha_flag_)
    {
        fprintf(f, "alpha=%lf\n", alpha_);
    }


    if (L_flag_){
        for (int axis = 0; axis < dim(); axis++){
            fprintf(f, "x%d_lo=%lf\n",axis,lo_hi(axis,0));
            fprintf(f, "x%d_hi=%lf\n",axis,lo_hi(axis,1));
        }
    }

    if (periodic_flag_){
        for (int axis = 0; axis < dim(); axis++){
            fprintf(f, "x%d_periodic=%d\n",axis,periodic(axis));
        }
    }

    if (seed_mass_flag_){
        fprintf(f, "seedMass=%lf\n", seed_mass_);
    }

    if (columns_flag_){
        fprintf(f, "columns=%d\n", columns_);
    }

    //fprintf(f, "id,x,y,z,assignedSeedStatus,currentSeedStatus,diameter,attachments,");

    fprintf(f, "id,");

    for (int axis = 0; axis < dim(); axis++)
        fprintf(f, "x%d,", axis);
    
    fprintf(f, "assignedSeedStatus,");
    fprintf(f, "currentSeedStatus,");
    fprintf(f, "diameter,");
    fprintf(f, "attachments,");

    for (int att = 1; att <= maxAttachments(); att++){
        if (att == maxAttachments())
            fprintf(f, "att_%d\n", att);
        else
            fprintf(f, "att_%d,", att);
    }

    for (int i = 0; i < numParticles(); i++){

        fprintf(f, "%d,", i);

        for (int axis = 0; axis < dim(); axis++)
            fprintf(f, "%lf,", pos(i,axis));

        fprintf(f,"%d,",original_seed(i));
        fprintf(f,"%d,",current_seed(i));
        fprintf(f,"%lf,",diameter(i));
        fprintf(f,"%d,",numAttachments(i));

        num_atts = numAttachments(i);

        for (int j = 0; j < maxAttachments(); j++){

            if (num_atts == maxAttachments()){

                if (j != (num_atts-1))
                    fprintf(f, "%d,", attachment(i,j));
                else
                    fprintf(f, "%d\n", attachment(i,j));

            }

            else {

                if (j < num_atts)
                    fprintf(f, "%d,", attachment(i,j));
                else if ((j >= num_atts) && (j != (maxAttachments()-1)))
                    fprintf(f, "NaN,");
                else
                    fprintf(f, "NaN\n");

            }

        }



    }

    fclose(f);
    free(filepath_);

}

void postprocessing::save_unfolded_config()
{
    FILE *f;
    int num_atts;
    char filename[] = "unfolded_config.csv";
  
    create_filepath(folder_name_, filename);
    f= fopen(filepath_,"w");

    get_headers();

    fprintf(f, "headers=%d\n", headers_);
    
    if (lattice_flag_){                         
        fprintf(f, "lattice=%d\n",lattice_);
    }

    if (N_flag_){
        fprintf(f, "N=%d\n", numParticles());
    }

    if (D_flag_){
        fprintf(f, "D=%d\n", dim());
    }

    if (max_attachments_flag_){
        fprintf(f, "maxAttachments=%d\n", maxAttachments());
    }

    if (folded_flag_)
        fprintf(f, "folded=0\n");

    if (phi_flag_)
        fprintf(f, "phi=%lf\n", phi_);


    if (iters_flag_)
    {
        fprintf(f, "iters=%d\n", iters_);
    }

    if (alpha_flag_)
    {
        fprintf(f, "alpha=%lf\n", alpha_);
    }


    if (L_flag_){
        for (int axis = 0; axis < dim(); axis++){
            fprintf(f, "x%d_lo=%lf\n",axis,lo_hi(axis,0));
            fprintf(f, "x%d_hi=%lf\n",axis,lo_hi(axis,1));
        }
    }

    if (periodic_flag_){
        for (int axis = 0; axis < dim(); axis++){
            fprintf(f, "x%d_periodic=%d\n",axis,periodic(axis));
        }
    }

    if (seed_mass_flag_){
        fprintf(f, "seedMass=%lf\n", seed_mass_);
    }

    if (columns_flag_){
        fprintf(f, "columns=%d\n", columns_);
    }

    //fprintf(f, "id,x,y,z,assignedSeedStatus,currentSeedStatus,diameter,attachments,");

    fprintf(f, "id,");

    for (int axis = 0; axis < dim(); axis++)
        fprintf(f, "x%d,", axis);
    
    fprintf(f, "assignedSeedStatus,");
    fprintf(f, "currentSeedStatus,");
    fprintf(f, "diameter,");
    fprintf(f, "attachments,");

    for (int att = 1; att <= maxAttachments(); att++){
        if (att == maxAttachments())
            fprintf(f, "att_%d\n", att);
        else
            fprintf(f, "att_%d,", att);
    }

    for (int i = 0; i < numParticles(); i++){

        fprintf(f, "%d,", i);

        for (int axis = 0; axis < dim(); axis++)
            fprintf(f, "%lf,", unfolded_coords(i,axis));

        fprintf(f,"%d,",original_seed(i));
        fprintf(f,"%d,",current_seed(i));
        fprintf(f,"%lf,",diameter(i));
        fprintf(f,"%d,",numAttachments(i));

        num_atts = numAttachments(i);

        for (int j = 0; j < maxAttachments(); j++){

            if (num_atts == maxAttachments()){

                if (j != (num_atts-1))
                    fprintf(f, "%d,", attachment(i,j));
                else
                    fprintf(f, "%d\n", attachment(i,j));

            }

            else {

                if (j < num_atts)
                    fprintf(f, "%d,", attachment(i,j));
                else if ((j >= num_atts) && (j != (maxAttachments()-1)))
                    fprintf(f, "NaN,");
                else
                    fprintf(f, "NaN\n");

            }

        }



    }

    fclose(f);
    free(filepath_);
}



}