#include <dlma_system.hh>
#include <dlma_system_onlattice.hh>

#ifndef SAVE_CONFIG_H
#define SAVE_CONFIG_H

namespace simulation{

template <typename type>
class save_config{

    public:

        virtual void save_configuration() {};
        virtual void save_configuration(char *filename) {};


};

}

#endif