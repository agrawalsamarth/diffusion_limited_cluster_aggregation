#include <on_lattice.hh>

namespace simulation{

on_lattice::on_lattice(const int dim, int *box_lengths, std::vector<boundary_conditions*> system_bc){

    D = dim;
    L = (int*)malloc(sizeof(int) * D);

    temp_pos      = (int*)malloc(sizeof(int) * D);
    neighbour_pos = (int*)malloc(sizeof(int) * D);

    for (int axis = 0; axis < D; axis++)
        L[axis] = box_lengths[axis];

    L_eff = (int*)malloc(sizeof(int) * D);

    for (int axis = 0; axis < D; axis++)
        L_eff[axis] = 1;

    for (int axis = 0; axis < D; axis++)
    {
        for (int itr = axis+1; itr < D; itr++)
            L_eff[axis] *= L[itr];
    }

    int L_total = 1;

    for (int axis = 0; axis < D; axis++)
        L_total *= L[axis];

    grid = (int*)malloc(sizeof(int) * L_total);

    for (int i = 0; i < L_total; i++)
        grid[i] = -1;

    for (int axis = 0; axis < D; axis++)
        box_bc.push_back(system_bc[axis]);

 
}

/*void on_lattice::set_L(const int L_val, const int axis){
    L[axis] = L_val;
}

int on_lattice::get_L(const int axis){

    return L[axis];    

}*/

int on_lattice::get_refill(int x, int axis){

    return (box_bc[axis]->refill(x, L[axis]));

}

/*void on_lattice::add_bc(boundary_conditions *bc){

    box_bc.push_back(bc);

}*/

void on_lattice::add_particle_to_cell(const int id, int *pos){

    counter = 0;

    for (int axis = 0; axis < D; axis++)
            counter += pos[axis] * L_eff[axis];

    grid[counter] = id;

}

void on_lattice::remove_particle_from_cell(const int id, int *pos){

    counter = 0;

    for (int axis = 0; axis < D; axis++)
            counter += pos[axis] * L_eff[axis];

    grid[counter] = -1;

}

int on_lattice::get_particle_id(int *pos){

    counter = 0;

    for (int axis = 0; axis < D; axis++)
            counter += pos[axis] * L_eff[axis];

    return grid[counter];


}

std::vector<int> on_lattice::get_neighbour_list(int *pos){

    neighbours.clear();

    for (int axis = 0; axis < D; axis++){
        for (int i = 0; i < 2; i++){

            for (int axis_2 = 0; axis_2 < D; axis_2++)
                temp_pos[axis_2] = (axis_2 == axis) * (-1 + 2*i);

            for (int axis_2 = 0; axis_2 < D; axis_2++){
                neighbour_pos[axis_2]  = (pos[axis_2] + temp_pos[axis_2]);
                neighbour_pos[axis_2] += (L[axis_2] * (neighbour_pos[axis_2]==-1)) - (L[axis_2] * (neighbour_pos[axis_2]==L[axis_2]));
            }

            neighbours.push_back(get_particle_id(neighbour_pos));            

        }
    }

}


}