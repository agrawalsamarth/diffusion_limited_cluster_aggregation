#include <dlca_lattice_3d.h>

void dlca_lattice_3d::get_max_att()
{

    max_attachments = 0;

    for (int i = 0; i < N; i++){
        if (getNumAttachments(i) > max_attachments)
            max_attachments = getNumAttachments(i);
    }

}

void dlca_lattice_3d::save_config(char *filename)
{
    FILE *f;
    //char filename[] = "../simulation_results/config.csv";
    f= fopen(filename,"w");
    int num_atts;
    get_max_att();

    headers = 11+(3*D);

    fprintf(f, "headers=%d\n", headers);
    
    //if (lattice_flag_){                         
        fprintf(f, "lattice=%d\n",lattice);
    //}

    //if (N_flag_){
        fprintf(f, "N=%d\n", N);
    //}

    //if (D_flag_){
        fprintf(f, "D=%d\n", D);
    //}

    //if (max_attachments_flag_){
        fprintf(f, "maxAttachments=%d\n", max_attachments);
    //}

    //if (folded_flag_)
        fprintf(f, "folded=%d\n", 1);

    //if (phi_flag_)
        fprintf(f, "phi=%lf\n", phi);


    //if (iters_flag_)
    //{
        fprintf(f, "iters=%d\n", iters);
    //}

    //if (alpha_flag_)
    //{
        fprintf(f, "alpha=%lf\n", alpha);
    //}


    //if (L_flag_){
        for (int axis = 0; axis < D; axis++){
            fprintf(f, "x%d_lo=%lf\n",axis,-0.5);
            fprintf(f, "x%d_hi=%lf\n",axis,L[axis]-0.5);
        }
    //}

    //if (periodic_flag_){
        for (int axis = 0; axis < D; axis++){
            fprintf(f, "x%d_periodic=%d\n",axis,periodic[axis]);
        }
    //}

    //if (seed_mass_flag_){
        fprintf(f, "seedMass=%lf\n", seed_mass);
    //}

    //if (columns_flag_){
        fprintf(f, "columns=%d\n", (5+D+max_attachments));
    //}

    //fprintf(f, "id,x,y,z,assignedSeedStatus,currentSeedStatus,diameter,attachments,");

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

        fprintf(f, "%d,", i);

        for (int axis = 0; axis < D; axis++)
            fprintf(f, "%d,", pos(i,axis));

        fprintf(f,"%d,",origSeedStatus_[i]);
        fprintf(f,"%d,",seed_[i]);
        fprintf(f,"%lf,",radius_[i]);
        if (max_attachments > 0)
            fprintf(f,"%d,",getNumAttachments(i));
        else
            fprintf(f,"%d",getNumAttachments(i));

        num_atts = getNumAttachments(i);

        for (int j = 0; j < max_attachments; j++){

            if (num_atts == max_attachments){

                if (j != (num_atts-1))
                    fprintf(f, "%d,", getAttachments(i,j));
                else
                    fprintf(f, "%d", getAttachments(i,j));

            }

            else {

                if (j < num_atts)
                    fprintf(f, "%d,", getAttachments(i,j));
                else if ((j >= num_atts) && (j != (max_attachments-1)))
                    fprintf(f, "NaN,");
                else
                    fprintf(f, "NaN");

            }

        }

        fprintf(f,"\n");



    }

    fclose(f);
    //free(filename);

}

void dlca_lattice_3d::save_config()
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

}