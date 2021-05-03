#include "post.h"

namespace post_p{

void postprocessing::read_params()
{
    FILE   *f;
    int    int_temp;
    double double_temp;
    int    fscanf_test;

    char filename[200];

    //f = fopen("../processed_data/N.csv", "r");
    f = fopen("./parsed_file/N.csv","r");
    fscanf_test = fscanf(f, "%d", &int_temp);
    N_ = int_temp;
    fclose(f);

    //f = fopen("../processed_data/D.csv", "r");
    f = fopen("./parsed_file/D.csv","r");
    fscanf_test = fscanf(f, "%d", &int_temp);
    D_ = int_temp;
    fclose(f);

    //f = fopen("../processed_data/max_attachments.csv", "r");
    f = fopen("./parsed_file/max_attachments.csv","r");
    fscanf_test = fscanf(f, "%d", &int_temp);
    max_attachments_ = int_temp;
    fclose(f);

    L_           = (double*)malloc(sizeof(double) * dim());
    halfL_       = (double*)malloc(sizeof(double) * dim());
    periodic_    = (int*)malloc(sizeof(int) * dim());
    posDiff_     = (double*)malloc(sizeof(double) * dim());

    //f = fopen("../processed_data/L.csv", "r");
    f = fopen("./parsed_file/L.csv","r");

    for (int axis = 0; axis < dim(); axis++){
        fscanf_test = fscanf(f, "%lf\n", &double_temp);
        L(axis)     = double_temp;
        halfL(axis) = 0.5 * double_temp;
    }

    fclose(f);

    //f = fopen("../processed_data/periodic.csv", "r");
    f = fopen("./parsed_file/periodic.csv","r");

    for (int axis = 0; axis < dim(); axis++){
        fscanf_test = fscanf(f, "%d\n", &int_temp);
        periodic(axis) = int_temp;
    }

    fclose(f);


}

}