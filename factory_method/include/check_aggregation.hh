#include "normal_bind.hh"
#include "aggregation_condition.hh"

#ifndef CHECK_AGGREGATION_H
#define CHECK_AGGREGATION_H

namespace simulation{

class check_aggregation{

    public:

        check_aggregation(dlma_system *system_state, normal_bind *bind_system, aggregation_condition *ref_condition);
        void check_for_aggregation(constituent<int> *c_1);

    private:

        dlma_system *sys_state;
        normal_bind *bind_sys;
        aggregation_condition *condition;
        std::vector<int> neighbours;
        constituent<int>* temp;

        int neighbour_id;
        int neighbour_cluster_id;
        int cluster_id;

        constituent<int>* particle_1;
        constituent<int>* particle_2;



};


}

#endif