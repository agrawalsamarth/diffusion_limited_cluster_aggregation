#include "normal_bind.hh"
#include "aggregation_condition.hh"

#ifndef CHECK_AGGREGATION_H
#define CHECK_AGGREGATION_H

namespace simulation{

template <typename type>
class check_aggregation{

    public:

        check_aggregation(system<type> *system_state, normal_bind<type> *bind_system, aggregation_condition<type> *ref_condition);
        void check_for_aggregation(constituent<type> *c_1);

    private:

        system<type> *sys_state;
        normal_bind<type> *bind_sys;
        aggregation_condition<type> *condition;
        std::vector<int> neighbours;
        constituent<type>* temp;

        int neighbour_id;
        int neighbour_cluster_id;
        int cluster_id;

        constituent<type>* particle_1;
        constituent<type>* particle_2;



};


}

#endif