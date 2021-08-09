#include "system_factory.hh"

#ifndef DLMA_SYSTEM_H
#define DLMA_SYSTEM_H

namespace simulation{

template <typename type>
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
        void iteration_step();
        void run_system();
        

    private:

        system_factory factory;
        simulation_box *box;

        vector<constituent<type>*> all_particles;
        vector<constituent<type>*> aggregates;

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
        int    *periodic;
        bool   *bc_flag;

};



}

#endif