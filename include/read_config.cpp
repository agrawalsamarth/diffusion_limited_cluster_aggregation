#include "post.h"

namespace post_p{

void postprocessing::read_config(){

    FILE   *f;
    double double_temp;
    int    int_temp;
    int    fscanf_test;

    char filename[500];

    strcpy(filename, "lo_hi.csv");
    create_filepath(parsed_folder_, filename);
    f = fopen(filepath_,"r");

    double lo_limit;
    double hi_limit;

    if (f != NULL){

        for (int axis = 0; axis < dim(); axis++){
            fscanf_test = fscanf(f, "%lf,%lf\n", &lo_limit, &hi_limit);
            lo_hi(axis, 0) = lo_limit;
            lo_hi(axis, 1) = hi_limit;
            L(axis)        = (hi_limit - lo_limit);
            halfL(axis)    = 0.5 * (hi_limit - lo_limit);
        }

        L_flag_ = true;
        fclose(f);
    }

    free(filepath_);

    //std::cout<<"9"<<std::endl;

    strcpy(filename, "periodic.csv");
    create_filepath(parsed_folder_, filename);
    f = fopen(filepath_,"r");

    if (f != NULL){

        for (int axis = 0; axis < dim(); axis++){
            fscanf_test = fscanf(f, "%d\n", &int_temp);
            periodic(axis) = int_temp;
        }

        periodic_flag_ = true;
        fclose(f);
    }

    free(filepath_);
 
    strcpy(filename, "config.csv");
    create_filepath(parsed_folder_, filename);
    f = fopen(filepath_,"r");

    for (int i = 0; i < numParticles(); i++){
        for (int axis = 0; axis < dim(); axis++){

            if (axis == (dim()-1)){
                fscanf_test = fscanf(f, "%lf\n", &double_temp);
                pos(i,axis) = double_temp;
            }

            else{
                fscanf_test = fscanf(f, "%lf,", &double_temp);
                pos(i,axis) = double_temp;
            }


        }

    }

    fclose(f);
    free(filepath_);

    //std::cout<<"2"<<std::endl;


    strcpy(filename, "num_attachments.csv");
    create_filepath(parsed_folder_, filename);
    f = fopen(filepath_,"r");

    for (int i = 0; i < numParticles(); i++){

        fscanf_test = fscanf(f, "%d\n", &int_temp);
        numAttachments(i) = int_temp;

    }

    fclose(f);
    free(filepath_);

    //std::cout<<"3"<<std::endl;

    strcpy(filename, "original_seed.csv");
    create_filepath(parsed_folder_, filename);
    f = fopen(filepath_,"r");

    for (int i = 0; i < numParticles(); i++){

        fscanf_test = fscanf(f, "%d\n", &int_temp);
        original_seed(i) = int_temp;

    }

    fclose(f);
    free(filepath_);

    //std::cout<<"4"<<std::endl;


    strcpy(filename, "current_seed.csv");
    create_filepath(parsed_folder_, filename);
    f = fopen(filepath_,"r");

    for (int i = 0; i < numParticles(); i++){

        fscanf_test = fscanf(f, "%d\n", &int_temp);
        current_seed(i) = int_temp;

    }

    fclose(f);
    free(filepath_);

    //std::cout<<"5"<<std::endl;


    strcpy(filename, "diameter.csv");
    create_filepath(parsed_folder_, filename);
    f = fopen(filepath_,"r");

    for (int i = 0; i < numParticles(); i++){

        fscanf_test = fscanf(f, "%lf\n", &double_temp);
        diameter(i) = double_temp;

    }

    fclose(f);
    free(filepath_);

    //std::cout<<"6"<<std::endl;


    for (int att = 0; att < maxAttachments(); att++) {

        sprintf(filename, "att_%d.csv",(att+1));
        create_filepath(parsed_folder_, filename);
        f = fopen(filepath_,"r");

        for (int i = 0; i < numParticles(); i++){

            fscanf_test = fscanf(f, "%d\n", &int_temp);
            attachment(i,att) = int_temp;

        }

        fclose(f);
        free(filepath_);

    }


}

}