#include "post.h"

namespace post_p{

void postprocessing::read_config(){

    FILE   *f;
    double double_temp;
    int    int_temp;
    char   filename[200];
    int    fscanf_test;

    pos_             = (double*)malloc(sizeof(double) * numParticles() * dim());
    num_attachments_ = (int*)malloc(sizeof(int) * numParticles());
    attachment_      = (int*)malloc(sizeof(int) * numParticles() * maxAttachments());

    //f = fopen("../processed_data/config.csv", "r");
    f = fopen("./parsed_file/config.csv", "r");

    for (int i = 0; i < numParticles(); i++){
        for (int axis = 0; axis < dim(); axis++){

            if (axis == (dim()-1)){
                fscanf_test = fscanf(f, "%lf\n", &double_temp);
                //std::cout<<double_temp<<std::endl;
                pos(i,axis) = double_temp;
            }

            else{
                fscanf_test = fscanf(f, "%lf,", &double_temp);
                //std::cout<<double_temp<<std::endl;
                pos(i,axis) = double_temp;
            }


        }

    }

    fclose(f);

    //f = fopen("../processed_data/num_attachments.csv", "r");
    f = fopen("./parsed_file/num_attachments.csv", "r");

    for (int i = 0; i < numParticles(); i++){

        fscanf_test = fscanf(f, "%d\n", &int_temp);
        numAttachments(i) = int_temp;

    }

    fclose(f);

    for (int att = 0; att < maxAttachments(); att++) {

        //sprintf(filename, "../processed_data/att_%d.csv",(att+1));
        sprintf(filename, "./parsed_file/att_%d.csv",(att+1));
        f = fopen(filename, "r");

        for (int i = 0; i < numParticles(); i++){

            fscanf_test = fscanf(f, "%d\n", &int_temp);
            attachment(i,att) = int_temp;

        }

        fclose(f);

    }


}

}