#include "cell3d.h"

#ifndef DLCA_LATTICE_H
#define DLCA_LATTICE_H

class dlca_lattice_3d
{
  public:

    dlca_lattice_3d(char *params_name)
    {
        read_params_parser(params_name);
        check_params();
        grid.initArrays(N, D, L);

        D_eff  = (2*D);

        pos_            = (int*)malloc(sizeof(int) * N * D);
        seed_           = (int*)malloc(sizeof(int) * N);
        origSeedStatus_ = (int*)malloc(sizeof(int) * N);
        check_status    = (bool*)malloc(sizeof(bool) * N);
        location        = (int*)malloc(sizeof(int) * D);
        nb_location     = (int*)malloc(sizeof(int) * D);
        movements       = (int*)malloc(sizeof(int) * D);

        clusterNumber_ = (int*)malloc(sizeof(int) * N);
        mass_          = (double*)malloc(sizeof(double)*N);
        propensity_    = (double*)malloc(sizeof(double)*N);
        cumPropensity_ = (double*)malloc(sizeof(double)*N);

        attachments_      = (int*)malloc(sizeof(int)*N*D_eff);
        attachmentCounts_ = (int*)malloc(sizeof(int)*N);
        radius_           = (double*)malloc(sizeof(double)*N);

        generator.seed(rng_seed);
        dis.param(std::uniform_real_distribution<double>::param_type(0.0, 1.0));

        totalClusters_ = N_s;
        totalWalkers_  = N - N_s;

        baseMass_[0] = 1.;
        baseMass_[1] = seed_mass;

        agg_threshold = (baseMass_[0] + baseMass_[1] - 0.1);

        initArrays();

        reset_check_status();

        for (int i = 0; i < N; i++){
            checkForAggregations(i);
        }

        reset_check_status();
        iters=0;

    }

    ~dlca_lattice_3d()
    {
        free(pos_);
        free(seed_);
        free(clusterNumber_);
        free(mass_);
        free(propensity_);
        free(cumPropensity_);
        free(attachments_);
        free(attachmentCounts_);

    }

    std::vector<std::string> split_string_by_delimiter(const std::string& s, char delimiter);
    void read_params_parser(char *config_filename);
    void save_config(char *filename);
    void save_config();
    void dump_xyz_file();
    void dump_xyz_file(const char *filename);
    void get_max_att();
    void check_params();
    void read_config_parser(char *config_filename);
    void force_moves(char *move_file);


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

    int
    getPeriodicImage(int x, int axis)
    {return (x + periodic[axis] * (L[axis]*(x==-1) - L[axis]*(x==L[axis]))); }

    int
    totalWalkers()
    {return totalWalkers_;}

    int
    totalClusters()
    {return totalClusters_;}

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

        for (int axis = 0; axis < D; axis++){
            location[axis] = L[axis]/2;
            pos(0,axis)    = location[axis];
        }

        grid.occupied(location) = 0;

        for (int i = 1; i < N; i++){

            flag = 0;

            while (flag == 0){

                for (int axis = 0; axis < D; axis++)
                    location[axis] = (int)(L[axis] * dis(generator)); 

                curr_index   = grid.occupied(location);

                if (curr_index == N) {
                    
                    for (int axis = 0; axis < D; axis++)
                        pos(i,axis) = location[axis];

                    grid.occupied(location) = i;
                    flag = 1;

                }

            }

        }

        calculatePropensity();

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

        reset_check_status();

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
                propensity_[i] = std::pow(mass_[i], alpha);
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

        temp_rand = 1. * D * dis(generator);
        axis_rand = (int)(temp_rand);

        temp_rand = dis(generator);
        sign_rand = (temp_rand < 0.5) - (temp_rand >= 0.5);

        for (int axis = 0; axis < D; axis++)
            movements[axis] = sign_rand * (axis == axis_rand);

        int flag = 0;

        for (int i = 0; i < N; i++){
            if (clusterNumber_[i] == index){

                for (int axis = 0; axis < D; axis++)
                    location[axis] = getPeriodicImage(pos(i,axis)+movements[axis],axis);
                
                curr_index   = grid.occupied(location);

                if (curr_index != N){
                    if (clusterNumber_[curr_index] != index)
                        flag = 1;
                }
                    


            }
        }

        if (flag == 0){

            for (int i = 0; i < N; i++) {

                if (clusterNumber_[i] == index){

                    for (int axis = 0; axis < D; axis++)
                        location[axis] = pos(i,axis);

                    grid.occupied(location) = N;
                    pos(i, axis_rand) = getPeriodicImage(pos(i,axis_rand)+sign_rand,axis_rand);

                }
            }

        }


        for (int i = 0; i < N; i++)
        {
            if (clusterNumber_[i] == index){

                for (int axis = 0; axis < D; axis++)
                    location[axis] = pos(i,axis);

                grid.occupied(location) = i;

                checkForAggregations(i);
            }

        }

        reset_check_status();


    }

    void
    reset_check_status()
    {

        for (int i = 0; i < N; i++)
            check_status[i] = false;

    }


    void
    bindAggregates(const int index_1, const int index_2)
    {

        clusterIndex_1 = clusterNumber_[index_1];
        clusterIndex_2 = clusterNumber_[index_2];

        mass_[clusterIndex_1] += mass_[clusterIndex_2];
        mass_[clusterIndex_2]  = 0.;

        for (int i = 0; i < N; i++){
            if (clusterNumber_[i] == clusterIndex_2)
                clusterNumber_[i] = clusterIndex_1;
        }

        if (seed_[index_1] == 0)
            totalWalkers_--;

        if (seed_[index_2] == 0)
            totalWalkers_--;

        if ((seed_[index_1] == 0) && (seed_[index_2] == 0))
            totalClusters_++;

        if ((seed_[index_1] == 1) && (seed_[index_2] == 1))
            totalClusters_--;

        seed_[index_1] = 1;
        seed_[index_2] = 1;

        att_count = attachmentCounts_[index_1];
        attachments_[index_1 * D_eff + att_count] = index_2;
        attachmentCounts_[index_1] += 1;

        att_count = attachmentCounts_[index_2];
        attachments_[index_2 * D_eff + att_count] = index_1;
        attachmentCounts_[index_2] += 1;

        calculatePropensity(); 


    }


    void
    checkForAggregations(int index_1) {

        for (int axis = 0; axis < D; axis++)
            location[axis] = pos(index_1,axis);
        
        clusterIndex_1 = clusterNumber_[index_1];

        for (int axis = 0; axis < D; axis++){
            for (int dir = 0; dir < 2; dir++){

                for (int axis_2 = 0; axis_2 < D; axis_2++)
                    nb_location[axis_2] = location[axis_2];

                sign_rand = 1 * (dir==0) - 1 * (dir==1);
                nb_location[axis] = getPeriodicImage(location[axis]+sign_rand, axis);

                index_2        = grid.occupied(nb_location);

                if (index_2 != N){
                    clusterIndex_2 = clusterNumber_[index_2];

                    if (clusterIndex_1 != clusterIndex_2) {

                        mass_sum = mass_[clusterIndex_1]+mass_[clusterIndex_2];

                        if (mass_sum > agg_threshold){
                            //std::cout<<"index1="<<index_1<<"\t index2="<<index_2<<std::endl;
                            bindAggregates(index_1, index_2);
                        }


                    }
                }
                
                
            }
        }

        check_status[index_1] = true;

        for (int axis = 0; axis < D; axis++){
            for (int dir = 0; dir < 2; dir++){

                for (int axis = 0; axis < D; axis++)
                    location[axis] = pos(index_1,axis);

                for (int axis_2 = 0; axis_2 < D; axis_2++)
                    nb_location[axis_2] = location[axis_2];

                sign_rand = 1 * (dir==0) - 1 * (dir==1);
                nb_location[axis] = getPeriodicImage(location[axis]+sign_rand, axis);

                index_2        = grid.occupied(nb_location);
                
                if ((index_2 != N) && (check_status[index_2] == false)){
                    checkForAggregations(index_2);
                }
                
                
            }
        }

        

    }

    void
    runSimulation() {

        //counter = 0;

        while ((totalWalkers_ != 0) || (totalClusters_ != 1)) {
            moveCluster();
            iters++;
            //std::cout<<"clusters = "<<totalClusters_<<"\t walkers = "<<totalWalkers_<<std::endl;
        }

    }

private:

    //double L;
    cell    grid;
    int     N;
    int     lattice;
    int     D;
    int    *L;
    int    *periodic;
    int    *location;
    int    *nb_location;
    bool   *check_status;
    int     N_s;
    double  seed_mass;
    double  alpha;
    double  agg_threshold;
    double  mass_sum;
    int     rng_seed;
    int    *pos_;
    int    *seed_;
    int    *origSeedStatus_;
    int    *clusterNumber_;
    int    totalClusters_;
    int    totalWalkers_;
    int    halfL_;
    double s[2];
    //int posDiff[D];
    double *clusters_S_;
    double *propensity_;
    int    clusterIndex_;
    int    clusterIndex_1;
    int    clusterIndex_2;
    double r2;
    double theta_;
    double phi;
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
    int    *movements;
    int    *attachments_;
    int    *attachmentCounts_;
    int    D_eff;
    int    att_count;
    int    index_1;
    int    index_2;
    double *radius_;
    double seed_pct;
    int    headers;
    int    max_attachments;
    int    iters;

    bool   N_flag         = false;
    bool   N_s_flag       = false;
    bool   lattice_flag   = false;
    bool   phi_flag       = false;
    bool   alpha_flag     = false;
    bool   seed_mass_flag = false;
    bool   rng_seed_flag  = false;
    bool   D_flag         = false;
    bool   seed_pct_flag  = false;
    bool   *L_flag;
    bool   L_flag_and;
    bool   L_flag_or;
    bool   *periodic_flag;
    bool   periodic_flag_and;


};

#endif
