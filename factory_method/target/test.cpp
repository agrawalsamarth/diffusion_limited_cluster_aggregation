//propensity
//params_file
//config_file
//off-lattice


#include <check_aggregation.hh>
#include <brownian_movement.hh>
#include <mass_aggregation_condition.hh>
#include <dlma_save_config.hh>

using namespace simulation;

int main(int argc, char *argv[])
{
    dlma_system test(argv[1]);
    normal_bind bind_test(&test);
    mass_aggregation_condition condition(&test);
    check_aggregation agg_test(&test, &bind_test, &condition);
    brownian_movement bm_test(2, 1729);
    dlma_save_config save_test(&test, test.get_box());

    int temp;
    constituent<int> *temp_c;

    for (int i = 0; i < test.get_latest_cluster_id_without_increment(); i++){

        temp_c = test.get_aggregate(i);

        if (temp_c){
            agg_test.check_for_aggregation(temp_c);
        }

    }

    while (test.total_aggregates() != 1){

        std::cout<<test.total_aggregates()<<std::endl;

        temp   = (int)(bm_test.get_rand() * test.total_aggregates());
        temp_c = test.get_constituent(temp); 
        test.move_aggregate(temp, bm_test.delta_x());
        agg_test.check_for_aggregation(temp_c);

    }

    save_test.save_configuration(argv[2]);

    return 0;

}