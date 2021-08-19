#include <mass_aggregation_condition.hh>

namespace simulation{

mass_aggregation_condition::mass_aggregation_condition(dlma_system *ref_sys)
{

    sys_state = ref_sys;

}

void mass_aggregation_condition::show_out()
{ std::cout<<"object"<<std::endl;}

bool mass_aggregation_condition::agg_condition(constituent<int> *c_1, constituent<int> *c_2)
{

    id_1 = c_1->get_id();
    id_2 = c_2->get_id();

    agg_1 = sys_state->get_id_map(id_1);
    agg_2 = sys_state->get_id_map(id_2);

    cluster_1 = sys_state->get_aggregate(agg_1);
    cluster_2 = sys_state->get_aggregate(agg_2);

    if ((cluster_1->get_mass() + cluster_2->get_mass()) >= sys_state->get_seedmass()){
        return true;
    }

    else
        return false; 


}




}