#include <dlma_save_config.hh>

namespace simulation{

dlma_save_config::dlma_save_config(dlma_system *ref_sys, simulation_box *ref_box)
{
    sys_state = ref_sys;
    box       = ref_box;
}

void dlma_save_config::save_configuration(char *filename)
{
    FILE *f;
    f= fopen(filename,"w");
    int num_atts;
    int max_attachments = sys_state->get_max_attachments();
    int lattice = sys_state->get_lattice();
    int N = sys_state->get_N();
    int D = sys_state->get_dim();
    
    double phi   = sys_state->get_phi();
    double alpha = sys_state->get_alpha();

    constituent<int> *temp;
    std::vector<int>  attachments;

    int headers = 10+(3*D);

    fprintf(f, "headers=%d\n", headers);
    fprintf(f, "lattice=%d\n", lattice);
    fprintf(f, "N=%d\n", N);
    fprintf(f, "D=%d\n", D);
    fprintf(f, "maxAttachments=%d\n", max_attachments);
    fprintf(f, "folded=%d\n", 1);
    fprintf(f, "phi=%lf\n", phi);
    fprintf(f, "alpha=%lf\n", alpha);

    for (int axis = 0; axis < D; axis++){
        fprintf(f, "x%d_lo=%lf\n",axis,0.0);
        fprintf(f, "x%d_hi=%lf\n",axis,1. * box->get_L(axis));
    }

    for (int axis = 0; axis < D; axis++){
        fprintf(f, "x%d_periodic=%d\n",axis,box->get_periodicity(axis));
    }

    fprintf(f, "seedMass=%lf\n", sys_state->get_seedmass());
    fprintf(f, "columns=%d\n", (5+D+max_attachments));


    fprintf(f, "id,");

    for (int axis = 0; axis < D; axis++)
        fprintf(f, "x%d,", axis);
    
    fprintf(f, "assignedSeedStatus,");
    fprintf(f, "currentSeedStatus,");
    fprintf(f, "diameter,");
    if (max_attachments > 0)
        fprintf(f, "attachments,");
    else
        fprintf(f, "attachments\n");
        
    for (int att = 1; att <= max_attachments; att++){
        if (att == max_attachments)
            fprintf(f, "att_%d\n", att);
        else
            fprintf(f, "att_%d,", att);
    }

    for (int i = 0; i < N; i++){

        temp        = sys_state->get_particle_by_id(i);
        attachments = sys_state->get_attachment_vector(i); 

        fprintf(f, "%d,", i);

        for (int axis = 0; axis < D; axis++){
            if (lattice = 1)
                fprintf(f, "%lf,", 1. * (temp->pos(axis)) + 0.5);
            else
                fprintf(f, "%lf,", temp->pos(axis));
        }

        fprintf(f,"%d,", temp->get_original_seed_status());
        fprintf(f,"%d,", temp->get_current_seed_status());
        fprintf(f,"%lf,",temp->get_diameter());

        num_atts = attachments.size();

        if (max_attachments > 0)
            fprintf(f,"%d,",num_atts);
        else
            fprintf(f,"%d",num_atts);

        

        for (int j = 0; j < max_attachments; j++){

            if (num_atts == max_attachments){

                if (j != (num_atts-1))
                    fprintf(f, "%d,", attachments[j]);
                else
                    fprintf(f, "%d", attachments[j]);

            }

            else {

                if (j < num_atts)
                    fprintf(f, "%d,", attachments[j]);
                else if ((j >= num_atts) && (j != (max_attachments-1)))
                    fprintf(f, "NaN,");
                else
                    fprintf(f, "NaN");

            }

        }

        fprintf(f,"\n");



    }

    fclose(f);

}

/*void dlca_lattice_3d::save_config()
{
    int num_atts;
    get_max_att();

    headers = 11+(3*D);

    printf("headers=%d\n", headers);
    
    //if (lattice_flag_){                         
        printf("lattice=%d\n",lattice);
    //}

    //if (N_flag_){
        printf("N=%d\n", N);
    //}

    //if (D_flag_){
        printf("D=%d\n", D);
    //}

    //if (max_attachments_flag_){
        printf("maxAttachments=%d\n", max_attachments);
    //}

    //if (folded_flag_)
        printf("folded=%d\n", 1);

    //if (phi_flag_)
        printf("phi=%lf\n", phi);


    //if (iters_flag_)
    //{
        printf("iters=%d\n", iters);
    //}

    //if (alpha_flag_)
    //{
        printf("alpha=%lf\n", alpha);
    //}


    //if (L_flag_){
        for (int axis = 0; axis < D; axis++){
            printf("x%d_lo=%lf\n",axis,-0.5);
            printf("x%d_hi=%lf\n",axis,L[axis]-0.5);
        }
    //}

    //if (periodic_flag_){
        for (int axis = 0; axis < D; axis++){
            printf("x%d_periodic=%d\n",axis,periodic[axis]);
        }
    //}

    //if (seed_mass_flag_){
        printf("seedMass=%lf\n", seed_mass);
    //}

    //if (columns_flag_){
        printf("columns=%d\n", (5+D+max_attachments));
    //}

    //fprintf(f, "id,x,y,z,assignedSeedStatus,currentSeedStatus,diameter,attachments,");

    printf("id,");

    for (int axis = 0; axis < D; axis++)
        printf("x%d,", axis);
    
    printf("assignedSeedStatus,");
    printf("currentSeedStatus,");
    printf("diameter,");
    if (max_attachments > 0)
        printf("attachments,");
    else
        printf("attachments\n");
        
    for (int att = 1; att <= max_attachments; att++){
        if (att == max_attachments)
            printf("att_%d\n", att);
        else
            printf("att_%d,", att);
    }

    for (int i = 0; i < N; i++){

        printf("%d,", i);

        for (int axis = 0; axis < D; axis++)
            printf("%d,", pos(i,axis));

        printf("%d,",origSeedStatus_[i]);
        printf("%d,",seed_[i]);
        printf("%lf,",radius_[i]);
        if (max_attachments > 0)
            printf("%d,",getNumAttachments(i));
        else
            printf("%d",getNumAttachments(i));

        num_atts = getNumAttachments(i);

        for (int j = 0; j < max_attachments; j++){

            if (num_atts == max_attachments){

                if (j != (num_atts-1))
                    printf("%d,", getAttachments(i,j));
                else
                    printf("%d", getAttachments(i,j));

            }

            else {

                if (j < num_atts)
                    printf("%d,", getAttachments(i,j));
                else if ((j >= num_atts) && (j != (max_attachments-1)))
                    printf("NaN,");
                else
                    printf("NaN");

            }

        }

        printf("\n");



    }

    //fclose(f);
    //free(filename);

}*/

}