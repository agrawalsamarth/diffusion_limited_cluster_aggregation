#include <save_config.hh>

#ifndef DLMA_SAVE_CONFIG_H
#define DLMA_SAVE_CONFIG_H

namespace simulation{


class dlma_save_config: public save_config{


    public:

        dlma_save_config(dlma_system *ref_sys, simulation_box *ref_box);
        void save_configuration();
        void save_configuration(char *filename);

    private:

        dlma_system    *sys_state;
        simulation_box *box;

};


}


#endif