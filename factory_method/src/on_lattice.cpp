#include <on_lattice.hh>

namespace simulation{

on_lattice::on_lattice(const int dim, int *box_lengths){

    D = dim;
    L = (int*)malloc(sizeof(int) * D);

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

 
}

void on_lattice::set_L(const int L_val, const int axis){
    L[axis] = L_val;
}

int on_lattice::get_L(const int axis){

    return L[axis];    

}

int on_lattice::get_refill(int x, int axis){

    return (box_bc[axis]->refill(x, L[axis]));

}

void on_lattice::add_bc(boundary_conditions *bc){

    box_bc.push_back(bc);

}

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


}