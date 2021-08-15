#include <dlma_system.hh>

#ifndef NORMAL_BIND_H
#define NORMAL_BIND_H

namespace simulation{

class normal_bind{

    public:

        normal_bind(dlma_system *system_ptr);
        void bind_aggregates(constituent<int> *c_1, constituent<int> *c_2);

    private:

        system_factory factory;
        dlma_system *sys_state;

};

}

#endif