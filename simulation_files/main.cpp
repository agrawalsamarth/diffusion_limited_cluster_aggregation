#include "dlca_lattice_2d.h"

int main(int argc, char *argv[])
{
    int file_number = atoi(argv[1]);
    //std::cout<<"start"<<std::endl;
    dlca_lattice_2d test(file_number);
    test.runSimulation();
    //test.checkAdjacency();

    int N = dlca_lattice_2d::N;
    int D = dlca_lattice_2d::D;
    int L = dlca_lattice_2d::L;

    //for (int i = 0; i < N; i++)
        //std::cout<<test.pos(i,0)<<"\t"<<test.pos(i,1)<<"\t"<<test.pos(i,2)<<std::endl;

    //int fraction = (int)(100 * (1.000001 - test.s_fraction));

    char filename[200];
    sprintf(filename, "./config_files/%d.csv", file_number);

    FILE *f;
    f = fopen(filename, "w");

    int num_atts;

    if (D==3){

        fprintf(f, "headers=20\n");
        fprintf(f, "lattice=1\n");
        fprintf(f, "L=%d\n", L);
        fprintf(f, "N=%d\n", N);
        fprintf(f, "D=%d\n", D);
        fprintf(f, "rho=%lf\n", (1.*N)/(1.*L*L*L));

        for (int i = 0; i < D; i++){
            fprintf(f, "x%d_lo=%lf\n",i,-1.* 0.5);
            fprintf(f, "x%d_hi=%lf\n",i, 1.*L-0.5);
        }

        for (int i = 0; i < D; i++){
            fprintf(f, "x%d_periodic=1\n",i);
        }


        fprintf(f, "iters=%d\n", test.getIters());
        fprintf(f, "alpha=%lf\n", -0.5);
        fprintf(f, "seedMass=%lf\n", test.getSeedMass());

        int max_att = 0;

        for (int i = 0; i < N; i++)
        {
            if (test.getNumAttachments(i) > max_att)
                max_att = test.getNumAttachments(i);
        }

        fprintf(f, "maxAttachments=%d\n", max_att);
        fprintf(f, "columns=%d\n", 8+max_att);

        fprintf(f, "id,x,y,z,assignedSeedStatus,currentSeedStatus,diameter,attachments,");

        for (int i = 1; i <= max_att; i++){

            if (i == max_att){
                fprintf(f, "att_%d\n", i);
            }

            else{
                fprintf(f, "att_%d,",i);
            }

        }

        for (int i = 0; i < N; i++){

            fprintf(f, "%d,%d,%d,%d,%d,%d,%d,%d,", i,test.pos(i,0),test.pos(i,1),test.pos(i,2),test.origSeed(i),test.seedStatus(i),test.getRadius(i),
                    test.getNumAttachments(i));

            num_atts = test.getNumAttachments(i);

            for (int j = 0; j < max_att; j++){

                if (num_atts == max_att){

                    if (j != (num_atts-1))
                        fprintf(f, "%d,", test.getAttachments(i,j));
                    else
                        fprintf(f, "%d\n", test.getAttachments(i,j));

                }

                else {

                    if (j < num_atts)
                        fprintf(f, "%d,", test.getAttachments(i,j));
                    else if ((j >= num_atts) && (j != (max_att-1)))
                        fprintf(f, "NaN,");
                    else
                        fprintf(f, "NaN\n");

                }

            }
        }

    }

    else {

        fprintf(f, "headers=17\n");
        fprintf(f, "lattice=1\n");
        fprintf(f, "L=%d\n", L);
        fprintf(f, "N=%d\n", N);
        fprintf(f, "D=%d\n", D);
        fprintf(f, "rho=%lf\n", (1.*N)/(1.*L*L*L));

        for (int i = 0; i < D; i++){
            fprintf(f, "x%d_lo=%lf\n",i,-1.* 0.5);
            fprintf(f, "x%d_hi=%lf\n",i, 1.*L-0.5);
        }

        for (int i = 0; i < D; i++){
            fprintf(f, "x%d_periodic=1\n",i);
        }


        fprintf(f, "iters=%d\n", test.getIters());
        fprintf(f, "alpha=%lf\n", -0.5);
        fprintf(f, "seedMass=%lf\n", test.getSeedMass());

        int max_att = 0;

        for (int i = 0; i < N; i++)
        {
            if (test.getNumAttachments(i) > max_att)
                max_att = test.getNumAttachments(i);
        }

        fprintf(f, "maxAttachments=%d\n", max_att);
        fprintf(f, "columns=%d\n", 8+max_att);

        fprintf(f, "id,x,y,assignedSeedStatus,currentSeedStatus,diameter,attachments,");

        for (int i = 1; i <= max_att; i++){

            if (i == max_att){
                fprintf(f, "att_%d\n", i);
            }

            else{
                fprintf(f, "att_%d,",i);
            }


        }

        for (int i = 0; i < N; i++){

            fprintf(f, "%d,%d,%d,%d,%d,%d,%d,", i,test.pos(i,0),test.pos(i,1),test.origSeed(i),test.seedStatus(i),test.getRadius(i),
                    test.getNumAttachments(i));

            num_atts = test.getNumAttachments(i);

            for (int j = 0; j < max_att; j++){

                if (num_atts == max_att){

                    if (j != (num_atts-1))
                        fprintf(f, "%d,", test.getAttachments(i,j));
                    else
                        fprintf(f, "%d\n", test.getAttachments(i,j));

                }

                else {

                    if (j < num_atts)
                        fprintf(f, "%d,", test.getAttachments(i,j));
                    else if ((j >= num_atts) && (j != (max_att-1)))
                        fprintf(f, "NaN,");
                    else
                        fprintf(f, "NaN\n");

                }

            }
        }

    }

    fclose(f);

    /*test.calculateRij();
    sprintf(filename, "results/rij/rij_%d.csv", fraction);

    f = fopen(filename, "w");

    for (int i = 0; i < ((N*(N-1))/2); i++)
        fprintf(f, "%lf\n", test.get_rij(i));

    fclose(f);*/

    double r_min = 1.0;

    return 0;
}
