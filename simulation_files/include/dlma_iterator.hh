#include "system_iterator.hh"
#include <chrono>

#ifndef DLMA_ITERATOR_H
#define DLMA_ITERATOR_H

namespace simulation{

template <typename type>
class dlma_iterator: public system_iterator<type>{

    public:

        dlma_iterator(char *filename);
        void run_system();
        void iteration_step();
        void save_config_file();
        void save_config_file(char *filename);
        void create_movie_files(char *filename);
        void run_system_for_percolation();



    private:

        std::vector<std::string> split_string_by_delimiter(const std::string& s, char delimiter);

        system<type> *sys_state;
        normal_bind<type> *binding_obj;
        aggregation_condition<type> *agg_condition;
        check_aggregation<type> *aggregation_check_obj;
        particle_movement<type> *movement_test;
        save_config<type> *save_obj;

        iterator_factory<type> *factory;


        int temp;
        constituent<type> *temp_c;
        int final_aggregate_number = 1;

};



}



#endif