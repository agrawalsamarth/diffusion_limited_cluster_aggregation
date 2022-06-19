#include <dlma_system.hh>
#include <misc.hh>

#ifndef DLMA_SYSTEM_OFFLATTICE_H
#define DLMA_SYSTEM_OFFLATTICE_H

namespace simulation{

template <typename type>
class dlma_system_offlattice: public dlma_system<type>{

    public:

        dlma_system_offlattice(char *params_name);
        void initialize_system();
        void move_aggregate(int i, type *dr);
        void add_attachment(const int i, const int j);
        void build_attachment_list();
    
    private:

        void calc_rij();
        //bool check_rij();
        std::vector<coll_deets> build_collision_list(int i, double alpha, type *dr);
        coll_deets calc_quad_eqn(type *dr, double alpha, constituent<type> *ref_particle, constituent<type> *nb_particle);
        type fix_overlap(const int i, type *dr);
        constituent<type> *image;
        std::vector<int>   actual_list;
        std::vector<int>   image_list;
        std::vector<int>   neighbours;
        int iters = 0;
        double *rij;
        std::vector<coll_deets> bonds;
        std::vector<coll_deets> hs;




};



}

#endif