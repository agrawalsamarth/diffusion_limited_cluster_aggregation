#include <aggregation_condition.hh>

#ifndef MASS_AGGREGATION_CONDITION_H
#define MASS_AGGREGATION_CONDITION_H

namespace simulation{

class mass_aggregation_condition: public aggregation_condition{

    public:

        mass_aggregation_condition(dlma_system *ref_sys);
        bool agg_condition(constituent<int> * c_1, constituent<int> * c_2);
        void show_out();

    private:

        dlma_system *sys_state;
        int id_1;
        int id_2;
        int agg_1;
        int agg_2;
        constituent<int> *cluster_1;
        constituent<int> *cluster_2;


};



}


#endif