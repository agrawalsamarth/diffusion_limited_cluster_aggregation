#include <dlca_lattice_3d.h>

void dlca_lattice_3d::dump_xyz_file()
{
    printf("%d\n\n", N);

    for (int i = 0; i < N; i++){

        if (seed_[i] == 0){
            printf("W\t");
        }

        else{
            printf("S\t");
        }

        for (int axis = 0; axis < D; axis++){

            if (axis == (D-1))
                printf("%lf\n", 1.*pos(i,axis));

            else
                printf("%lf\t", 1.*pos(i,axis));

        }

    }

}

void dlca_lattice_3d::dump_xyz_file(const char *filename)
{
    FILE *f;
    f=fopen(filename, "w");

    fprintf(f,"%d\n\n", N);

    for (int i = 0; i < N; i++){

        if (seed_[i] == 0){
            fprintf(f,"Walker\t");
        }

        else{
            fprintf(f,"Seed\t");
        }

        for (int axis = 0; axis < D; axis++){

            //if (axis == (D-1))
                //fprintf(f, "%lf\n", 1.*pos(i,axis));

            //else
                fprintf(f, "%lf\t", 1.*pos(i,axis));

        }

	fprintf(f,"0.5\n");

    }

    fclose(f);

}
