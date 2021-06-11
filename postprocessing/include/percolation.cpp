#include "post.h"

namespace post_p
{

bool postprocessing::check_percolation()
{
    init_unfolding();
    bool percolation_test = false;

    for (int i = 0; i < totalClusters_; i++){
        for (int axis = 0; axis < dim(); axis++) {
            percolation_test = (percolation_test || cluster_percolation(i,axis));
        }
    }

    return percolation_test;

}

void postprocessing::dump_percolation_file(char *filename)
{

    FILE *f;

    f=fopen(filename, "w");

    fprintf(f,"cluster_number,");

    for (int axis = 0; axis < dim(); axis++){
        if (axis == (dim() -1))
            fprintf(f, "percolation_x%d\n",axis);
        else
            fprintf(f, "percolation_x%d,",axis);
    }

    for (int i = 0; i < totalClusters_; i++){
        fprintf(f, "%d,",i);
        for (int axis = 0; axis < dim(); axis++){
            if (axis == (dim() -1))
                fprintf(f, "%d\n",cluster_percolation(i,axis));
            else
                fprintf(f, "%d,",cluster_percolation(i,axis));
        }

    }

    fclose(f);

}

void postprocessing::dump_percolation_file()
{

    printf("cluster_number,");

    for (int axis = 0; axis < dim(); axis++){
        if (axis == (dim() -1))
            printf("percolation_x%d\n",axis);
        else
            printf("percolation_x%d,",axis);
    }

    for (int i = 0; i < totalClusters_; i++){
        printf("%d,",i);
        for (int axis = 0; axis < dim(); axis++){
            if (axis == (dim() -1))
                printf("%d\n",cluster_percolation(i,axis));
            else
                printf("%d,",cluster_percolation(i,axis));
        }

    }


}


}