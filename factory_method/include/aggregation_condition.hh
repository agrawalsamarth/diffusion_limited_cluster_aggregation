#include <dlma_system.hh>

#ifndef AGGREGATION_CONDITION_H
#define AGGREGATION_CONDITION_H

namespace simulation{

class aggregation_condition{

    public:

        virtual void show_out() {};
        virtual bool agg_condition(constituent<int> *c_1, constituent<int> *c_2) {};


};


}

#endif