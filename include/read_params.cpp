#include "post.h"

namespace post_p{

void postprocessing::read_params()
{
    FILE   *f;
    int    int_temp;
    double double_temp;
    int    fscanf_test;

    char  filename[500];
    
    strcpy(filename, "N.csv");
    create_filepath(parsed_folder_, filename);
    f = fopen(filepath_,"r");
    
    if (f != NULL){
        fscanf_test = fscanf(f, "%d", &int_temp);
        N_ = int_temp;
        N_flag_ = true;
        fclose(f);
    }

    free(filepath_);

    strcpy(filename, "D.csv");
    create_filepath(parsed_folder_, filename);
    f = fopen(filepath_,"r");
    
    if (f != NULL){
        fscanf_test = fscanf(f, "%d", &int_temp);
        D_ = int_temp;
        D_flag_ = true;
        fclose(f);
    }

    free(filepath_);

    strcpy(filename, "max_attachments.csv");
    create_filepath(parsed_folder_, filename);
    f = fopen(filepath_,"r");

    if (f != NULL){
        fscanf_test = fscanf(f, "%d", &int_temp);
        max_attachments_ = int_temp;
        max_attachments_flag_ = true;
        fclose(f);
    }

    free(filepath_);

    //std::cout<<"4"<<std::endl;

    strcpy(filename, "iters.csv");
    create_filepath(parsed_folder_, filename);
    f = fopen(filepath_,"r");

    if (f != NULL){

        fscanf_test = fscanf(f, "%d", &int_temp);
        iters_      = int_temp;
        iters_flag_ = true;
        fclose(f);

    }

    free(filepath_);

    //std::cout<<"5"<<std::endl;

    strcpy(filename, "folded.csv");
    create_filepath(parsed_folder_, filename);
    f = fopen(filepath_,"r");
    
    if (f != NULL){
    
        fscanf_test  = fscanf(f, "%d", &int_temp);
        folded_      = int_temp;
        folded_flag_ = true;
        fclose(f);

    }

    free(filepath_);
    
    //std::cout<<"6"<<std::endl;

    strcpy(filename, "alpha.csv");
    create_filepath(parsed_folder_, filename);
    f = fopen(filepath_,"r");
    
    if (f != NULL){

        fscanf_test = fscanf(f, "%lf", &double_temp);
        alpha_      = double_temp;
        alpha_flag_ = true;
        fclose(f);

    }

    free(filepath_);
    
    //std::cout<<"7"<<std::endl;

    strcpy(filename, "seedMass.csv");
    create_filepath(parsed_folder_, filename);
    f = fopen(filepath_,"r");
    
    if (f != NULL){

        fscanf_test     = fscanf(f, "%lf", &double_temp);
        seed_mass_      = double_temp;
        seed_mass_flag_ = true;
        fclose(f);

    }

    free(filepath_);
   

}

}