//#include <particle.hh>
//#include <cluster.hh>

#include <particle.hh>
#include <cluster.hh>

using namespace simulation;

int main()
{
    const int D = 2;

    boundary_conditions *bc_test_1 = new periodic_bc;
    boundary_conditions *bc_test_2 = new periodic_bc;

    int box_lengths[D];

    box_lengths[0] = 4;
    box_lengths[1] = 4;

    simulation_box *test = new on_lattice(D, box_lengths);

    test->add_bc(bc_test_1);
    test->add_bc(bc_test_2);

    
    constituent<int> *first          = new particle<int> (D, test);
    constituent<int> *second         = new particle<int> (D, test);
    constituent<int> *cluster_test   = new cluster<int>;

    int posDiff[D];

    for (int axis = 0; axis < D; axis++){
        first->pos(axis)  = (1 * axis);
        second->pos(axis) = (2 * axis);
        posDiff[axis]=2*axis;
    }

    cluster_test->add_constituent(first);
    cluster_test->add_constituent(second);

    //cluster_test->move(posDiff);

    cluster_test->add_constituent_to_cell();

    int pos[D];

    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++) {

            pos[0] = i;
            pos[1] = j;

            std::cout<<test->get_particle_id(pos)<<"\t";
        }

        std::cout<<"\n";

    }

    std::cout<<"\n\n\n";

    cluster_test->remove_constituent_from_cell();
    cluster_test->move(posDiff);
    cluster_test->add_constituent_to_cell();

    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++) {

            pos[0] = i;
            pos[1] = j;

            std::cout<<test->get_particle_id(pos)<<"\t";
        }

        std::cout<<"\n";

    }

    //delete first;
    //delete second;
    //delete test;

    return 0;

}