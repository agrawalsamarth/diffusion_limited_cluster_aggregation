#include "normal_bind.hh"

#ifndef CHECK_AGGREGATION_H
#define CHECK_AGGREGATION_H

namespace simulation{

class check_aggregation{

    public:

        void check_for_aggregation(constituent<int> *c_1);

    private:

        dlma_system *sys_state;
        normal_bind bind_sys;
        std::vector<int> neighbours;



};


}

#endif