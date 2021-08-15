#include "system_factory.hh"
#include <cstring>
#include <sstream>
#include <fstream>
#include <cmath>
#include <random>
#include <map>

#ifndef DLMA_SYSTEM_H
#define DLMA_SYSTEM_H

namespace simulation{

//template <typename type>
class dlma_system{

    public:

        dlma_system(char *params_name);

        std::vector<std::string> split_string_by_delimiter(const std::string& s, char delimiter);
        void read_params_parser(char *params_name);
        void check_params();
        void initialize_system();

        void check_aggregation();
        void move_constituent(const int i);
        void calculate_propensity();
        //void iteration_step();
        //void run_system();

        void add_aggregate(constituent<int> *new_aggregate);
        void remove_aggregate(const int id);

        void build_id_map();

        int get_latest_cluster_id();
        int get_id_map(constituent<int> *c_1);

        int get_lattice();
        int get_dim();

        simulation_box* get_box();

        constituent<int>* get_constituent(const int i);
        void print_id_map();

        

    private:

        std::mt19937 generator;
        std::uniform_real_distribution<double> dis;

        system_factory factory;
        simulation_box *box;

        std::vector<constituent<int>*> all_particles;
        std::vector<constituent<int>*> aggregates;

        std::map<int, int> id_map;

        double  *propensity;
        double  *cum_propensity;

        int    N;
        bool   N_flag = false;
        int    N_s;
        bool   N_s_flag = false;
        double seed_pct;
        bool   seed_pct_flag = false;
        int    D;
        int    D_flag = false;
        int    lattice;
        bool   lattice_flag = false;
        double phi;
        bool   phi_flag = false;
        double alpha;
        bool   alpha_flag = false;
        double seed_mass;
        bool   seed_mass_flag = false;
        int    rng_seed = 0;
        bool   rng_seed_flag = false;
        int    *L;
        bool   *L_flag;
        bool    L_flag_and;
        bool    L_flag_or;
        int    *periodic;
        bool   *periodic_flag;
        bool    periodic_flag_and;
        int     latest_cluster_id=0;

};



}

#endif