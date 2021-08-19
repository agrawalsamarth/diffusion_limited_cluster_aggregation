#include "system_iterator.hh"

#ifndef DLMA_ITERATOR_H
#define DLMA_ITERATOR_H

namespace simulation{

class dlma_iterator: public system_iterator{

    public:

        dlma_iterator(char *filename);
        void run_system();
        void iteration_step();
        void save_config_file();
        void save_config_file(char *filename);


    private:

        std::vector<std::string> split_string_by_delimiter(const std::string& s, char delimiter);

        dlma_system *sys_state;
        normal_bind *binding_obj;
        aggregation_condition *agg_condition;
        check_aggregation *aggregation_check_obj;
        particle_movement *movement_test;
        save_config *save_obj;

        iterator_factory *factory;


        int temp;
        constituent<int> *temp_c;

};



}



#endif