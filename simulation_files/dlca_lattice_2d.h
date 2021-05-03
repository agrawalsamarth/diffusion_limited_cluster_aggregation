#include "cell2d.h"
#include <random>

class dlca_lattice_2d
{
  public:

    static const int N  = cell2d::N;
    static const int D  = cell2d::D;
    static const int L  = cell2d::L;
    const double s_fraction = 0.1;
    //const int N_s = (int)(N * s_fraction);
    //const int N_s = 1;


    dlca_lattice_2d(int seed)
    {
        N_s = (int)(0.01 * N * seed);
	//N_s = 1;
        D_eff  = (2*D);

        //const int N_s = (int)(0.01 * N * seed);
        std::cout<<"Ns = "<<N_s<<std::endl;

        pos_            = (int*)malloc(sizeof(int) * N * D);
        seed_           = (int*)malloc(sizeof(int) * N);
        origSeedStatus_ = (int*)malloc(sizeof(int) * N);

        clusterNumber_ = (int*)malloc(sizeof(int) * N);
        mass_          = (double*)malloc(sizeof(double)*N);
        propensity_    = (double*)malloc(sizeof(double)*N);
        cumPropensity_ = (double*)malloc(sizeof(double)*N);

        attachments_      = (int*)malloc(sizeof(int)*N*D_eff);
        attachmentCounts_ = (int*)malloc(sizeof(int)*N);
        radius_           = (double*)malloc(sizeof(double)*N);

        int r_N = (N*(N-1))/2;

        r_ij_ = (double*)malloc(sizeof(double)*r_N);

        generator.seed(1);
        dis.param(std::uniform_real_distribution<double>::param_type(0.0, 1.0));

        totalClusters_ = N_s;
        totalWalkers_  = N - N_s;

        baseMass_[0] = 1.;
        baseMass_[1] = 1e12;

        initArrays();

        halfL_ = L/2;


    }

    ~dlca_lattice_2d()
    {
        free(pos_);
        free(seed_);
        free(clusterNumber_);
        free(mass_);
        free(propensity_);
        free(cumPropensity_);
        free(attachments_);
        free(attachmentCounts_);
        free(r_ij_);

    }

    int
    pos(const int i, const int axis) const
    { return pos_[i * D + axis]; }
    int&
    pos(const int i, const int axis)
    { return pos_[i * D + axis]; }

    int
    getSeed(const int i)
    { return seed_[i];  }

    double
    getPropensity(const int i)
    { return propensity_[i];}

    int
    getClusterNumber(const int i)
    { return clusterNumber_[i];}

    int
    seedStatus(const int i)
    {  return seed_[i]; }

    int
    origSeed(const int i)
    { return origSeedStatus_[i]; }

    double
    get_rij(const int i)
    { return r_ij_[i]; }

    int
    getAttachments(const int i, const int j)
    { return attachments_[i * D_eff + j]; }

    int
    getNumAttachments(const int i)
    { return attachmentCounts_[i]; }

    int
    getRadius(const int i)
    { return radius_[i]; }

    int
    getIters()
    { return counter; }

    double
    getSeedMass()
    { return baseMass_[1];}

    void
    initArrays()
    {
        int seedStatus;
        int flag;

        for (int i = 0; i < N; i++) {

            if (i < N_s)
                seedStatus        = 1;
            else
                seedStatus        = 0;

            seed_[i]          = seedStatus;
            clusterNumber_[i] = i;
            mass_[i]          = baseMass_[seedStatus];

        }

        std::cout<<"L = "<<L<<std::endl;

        x_ = L/2;
        y_ = L/2;

        pos(0,0) = x_;
        pos(0,1) = y_;

        //std::cout<<"x = "<<x_<<"\t y = "<<y_<<"\t z = "<<z_<<std::endl;

        grid.occupied(x_,y_) = 0;

        for (int i = 1; i < N; i++){

            flag = 0;

            while (flag == 0){

                x_ = (int)(L * dis(generator));
                y_ = (int)(L * dis(generator));

                left     = (x_-1) + (L * (x_==0));
                right    = (x_+1) - (L * (x_==(L-1)));
                down     = (y_-1) + (L * (y_==0));
                up       = (y_+1) - (L * (y_==(L-1)));

                curr_index   = grid.occupied(x_,y_);
                left_index   = grid.occupied(left,y_);
                right_index  = grid.occupied(right,y_);
                down_index   = grid.occupied(x_,down);
                up_index     = grid.occupied(x_,up);


                if ((curr_index == N) && (left_index == N) && (right_index == N) && (down_index == N) && (up_index == N)) {
                    pos(i, 0) = x_;
                    pos(i, 1) = y_;

                    grid.occupied(x_,y_) = i;
                    flag = 1;

                }

                        //std::cout<<"x = "<<x_<<"\t y = "<<y_<<"\t z = "<<z_<<std::endl;


            }

        }

        calculatePropensity();

        std::cout<<"here"<<std::endl;

        for (int i = 0; i < N; i++){
            attachmentCounts_[i] = 0;

            for (int j = 0; j < D_eff; j++){
                attachments_[i*D_eff+j] = N;
            }

        }

        for (int i = 0; i < N; i++)
            radius_[i] = 1.;

        for (int i = 0; i < N; i++)
            origSeedStatus_[i] = seed_[i];




    }

    void
    printPositions()
    {

        for (int i = 0; i < N; i++)
            std::cout<<pos(i,0)<<"\t"<<pos(i,1)<<"\t"<<seed_[i]<<std::endl;

    }

    void
    calculatePropensity()
    {
        totalPropensity_ = 0.;

        for (int i = 0; i < N; i++){

            if (mass_[i] < baseMass_[0]){
                propensity_[i] = 0.;
            }

            else{
                propensity_[i] = 1./std::sqrt(mass_[i]);
                //std::cout<<"sqrt = "<<std::sqrt(mass_[i])<<std::endl;
            }

            totalPropensity_ += propensity_[i];
            cumPropensity_[i] = totalPropensity_;

        }

    }

    void
    moveCluster()
    {
        temp_rand = 1. * totalPropensity_ * dis(generator);

        for (int i = 0; i < N; i++) {
            if (temp_rand < cumPropensity_[i]){
                index = i;
                break;
            }
        }

        //std::cout<<"index = "<<index<<std::endl;

        temp_rand = 2. * dis(generator);
        axis_rand = (0 * (temp_rand < 1.0)) + (1 * (temp_rand >= 1.0));

        temp_rand = dis(generator);
        sign_rand = (temp_rand < 0.5) - (temp_rand >= 0.5);

        for (int axis = 0; axis < D; axis++)
            movements[axis] = sign_rand * (axis == axis_rand);

        for (int i = 0; i < N; i++){
            if (clusterNumber_[i] == index){

                x_ = pos(i,0)+movements[0];
                y_ = pos(i,1)+movements[1];

                x_ += (L * (x_ == -1)) - (L * (x_ == L));
                y_ += (L * (y_ == -1)) - (L * (y_ == L));

                curr_index   = grid.occupied(x_,y_);

                if (curr_index == N)
                    curr_cluster = N;
                else
                    curr_cluster = clusterNumber_[curr_index];

                /*if (curr_index != N){
                        //j = grid.occupied(x_,y_,z_);
                        std::cout<<"WRONG counter = "<<counter<<"\t 1 = "<<pos(i,0)<<" "<<pos(i,1)<<" "<<pos(i,2)<<" cluster_2 = "<<
                        clusterNumber_[i]<<"\t 2 = "<<pos(curr_index,0)<<" "<<pos(curr_index,1)<<" "<<pos(curr_index,2)<<" cluster_1 = "<<
                        clusterNumber_[curr_index]<<std::endl;
                }*/

                if ((curr_index == N) || (clusterNumber_[i] == curr_cluster)){

                        //std::cout<<"here"<<std::endl;

                    x_ = pos(i,0);
                    y_ = pos(i,1);

                    //std::cout<<"init -- "<<x_<<" "<<y_<<" "<<z_<<"\t axis_rand = "<<axis_rand<<"\t sign_rand = "<<sign_rand<<std::endl;

                    grid.occupied(x_, y_) = N;

                    pos(i, axis_rand) += sign_rand;
                    pos(i, axis_rand) += (L * (pos(i, axis_rand) == -1)) - (L * (pos(i, axis_rand) == L));


                }


            }
        }

        //for (int axis = 0; axis < 6; axis++){
            for (int i = 0; i < N; i++)
            {
                if (clusterNumber_[i] == index){

                    x_ = pos(i,0);
                    y_ = pos(i,1);

                    grid.occupied(x_, y_) = i;

                    checkForAggregations(x_, y_, i);
                }

            }
        //}


    }

    void
    bindWalker (const int walker, const int cluster)
    {
        //std::cout<<"walker = "<<walker<<std::endl;

        seed_[walker] = 1;
        clusterNumber_[walker] = cluster;

        mass_[cluster] += mass_[walker];
        mass_[walker]   = 0.;

        calculatePropensity();

        //for (int i = 0; i < N; i++)
            //std::cout<<"propensity = "<<propensity_[i]<<std::endl;

    }

    void
    bindClusters (const int cluster_1, const int cluster_2)
    {
        //std::cout<<"here 2"<<std::endl;

        for (int i = 0; i < N; i++) {
            if (clusterNumber_[i] == cluster_2)
                clusterNumber_[i] = cluster_1;
        }

        mass_[cluster_1] += mass_[cluster_2];
        mass_[cluster_2]  = 0.;

        calculatePropensity();
    }

    void
    checkForNeighbour(const int i, const int j)
    {

            if (j != N){

                seed_2          = seed_[j];
                clusterIndex_2  = clusterNumber_[j];

                if (clusterIndex_1 != clusterIndex_2){

                    if ((seed_1 == 0) && (seed_2 == 1)){
                        bindWalker(i, clusterIndex_2);
                        totalWalkers_--;

                        att_count = attachmentCounts_[i];
                        attachments_[i * D_eff + att_count] = j;
                        attachmentCounts_[i] += 1;

                        att_count = attachmentCounts_[j];
                        attachments_[j * D_eff + att_count] = i;
                        attachmentCounts_[j] += 1;
                    }

                    if ((seed_1 == 1) && (seed_2 == 0)){
                        bindWalker(j, clusterIndex_1);
                        totalWalkers_--;

                        att_count = attachmentCounts_[i];
                        attachments_[i * D_eff + att_count] = j;
                        attachmentCounts_[i] += 1;

                        att_count = attachmentCounts_[j];
                        attachments_[j * D_eff + att_count] = i;
                        attachmentCounts_[j] += 1;

                    }

                    if ((seed_1 == 1) && (seed_2 == 1)){
                        bindClusters(clusterIndex_1, clusterIndex_2);
                        totalClusters_--;

                        att_count = attachmentCounts_[i];
                        attachments_[i * D_eff + att_count] = j;
                        attachmentCounts_[i] += 1;

                        att_count = attachmentCounts_[j];
                        attachments_[j * D_eff + att_count] = i;
                        attachmentCounts_[j] += 1;


                    }

                }
            }


    }

    void
    checkForAggregations(const int x, const int y, const int i) {

            left     = (x-1) + (L * (x==0));
            right    = (x+1) - (L * (x==(L-1)));
            down     = (y-1) + (L * (y==0));
            up       = (y+1) - (L * (y==(L-1)));

            seed_1         = seed_[i];
            clusterIndex_1 = clusterNumber_[i];

            neighbour_index = grid.occupied(left, y);
            checkForNeighbour(i, neighbour_index);

            seed_1         = seed_[i];
            clusterIndex_1 = clusterNumber_[i];

            neighbour_index = grid.occupied(right, y);
            checkForNeighbour(i, neighbour_index);

            seed_1         = seed_[i];
            clusterIndex_1 = clusterNumber_[i];

            neighbour_index = grid.occupied(x, down);
            checkForNeighbour(i, neighbour_index);

            seed_1         = seed_[i];
            clusterIndex_1 = clusterNumber_[i];

            neighbour_index = grid.occupied(x, up);
            checkForNeighbour(i, neighbour_index);


    }

    void
    checkAdjacency()
    {

        int r;


        for (int i = 0; i < N; i++){
            for (int j = i+1; j < N; j++) {

                r = 0;

                for (int axis = 0; axis < D; axis++)
                    posDiff[axis] = pos(i,axis)-pos(j,axis);

                //for (int axis = 0; axis < D; axis++)
                    //posDiff[axis] += (L * (posDiff[axis] <= -L/2)) - (L * (posDiff[axis] >= L/2));

                for (int axis = 0; axis < D; axis++)
                    r += (posDiff[axis]*posDiff[axis]);

                if (r == 0)
                    std::cout<<"i = "<<i<<"\t j = "<<j<<"\t counter = "<<counter<<std::endl;



            }
        }


    }

    void
    runSimulation() {

        counter = 0;

        //std::cout<<counter<<std::endl;

        while ((totalWalkers_ != 0) || (totalClusters_ != 1)) {
            moveCluster();
            //checkAdjacency();
            //printPositions();
            counter++;
            std::cout<<"clusters = "<<totalClusters_<<"\t walkers = "<<totalWalkers_<<std::endl;
            //std::cout<<"counter = "<<counter<<"\t 1 = "<<pos(3,0)<<" "<<pos(3,1)<<" "<<pos(3,2)<<" "<<clusterNumber_[3]<<"\t 2 = "<<
                            //pos(20,0)<<" "<<pos(20,1)<<" "<<pos(20,2)<<" "<<clusterNumber_[20]<<std::endl;
        }

    }

    void
    calculateRij()
    {
        int r_counter = 0;
        double r;

        for (int i = 0; i < N; i++){
            for (int j = i+1; j < N; j++){

                r = 0.;

                for (int axis = 0; axis < D; axis++)
                        posDiff[axis] = pos(i,axis)-pos(j,axis);

                for (int axis = 0; axis < D; axis++)
                        posDiff[axis] += (L * (posDiff[axis] < -halfL_)) - (L * (posDiff[axis] > halfL_));

                for (int axis = 0; axis < D; axis++)
                    r += (1. * posDiff[axis] * posDiff[axis]);

                r_ij_[r_counter] = sqrt(r);
                r_counter++;


            }
        }



    }

private:

    //double L;
    cell2d grid;
    int     N_s;
    int    *pos_;
    int    *seed_;
    int    *origSeedStatus_;
    int    *clusterNumber_;
    int    totalClusters_;
    int    totalWalkers_;
    int    halfL_;
    double s[2];
    int posDiff[D];
    double *clusters_S_;
    double *propensity_;
    int    clusterIndex_;
    int    clusterIndex_1;
    int    clusterIndex_2;
    double r2;
    double theta_;
    double phi_;
    int    x_;
    int    y_;
    int    z_;
    int    index;
    double rho;
    double *r_ij_;
    //double L;
    double *cumPropensity_;
    double totalPropensity_;
    double temp_rand;
    int    axis_rand;
    int    sign_rand;
    std::mt19937 generator;
    std::uniform_real_distribution<double> dis;

    double z_dis;
    double prefactor;
    int    counter;
    double baseMass_[2];
    double *mass_;
    int    seed_1;
    int    seed_2;
    int    left;
    int    right;
    int    down;
    int    up;
    int    behind;
    int    ahead;
    int    left_index;
    int    right_index;
    int    down_index;
    int    up_index;
    int    behind_index;
    int    ahead_index;
    int    curr_index;
    int    curr_cluster;
    int    neighbour_index;
    int    movements[D];
    int    *attachments_;
    int    *attachmentCounts_;
    int    D_eff;
    int    att_count;
    double *radius_;

};
