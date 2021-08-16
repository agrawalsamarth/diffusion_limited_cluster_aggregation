#include <iostream>
#include <vector>

#include "periodic_bc.hh"

#ifndef SIMULATION_BOX_H
#define SIMULATION_BOX_H

namespace simulation{


class simulation_box{

    public:

        virtual void add_bc(boundary_conditions* bc) {};
        virtual void set_L(const int L_val, const int axis) {};
        virtual int  get_L(const int axis) {};

        virtual int get_refill(int x, int axis) {};
        virtual int get_refill(int old_pos, int new_pos, int axis) {};

        virtual double get_refill(double x, int axis) {};
        virtual double get_refill(double old_pos, double new_pos, int axis) {};

        virtual double periodic_distance(double x, int axis) {};

        virtual void add_particle_to_cell(const int id, int *pos) {};
        virtual void remove_particle_from_cell(const int id, int *pos) {};
        virtual int  get_particle_id(int *pos) {};

        virtual void add_particle_to_cell(const int id, double *pos) {};
        virtual void remove_particle_from_cell(const int id, double *pos) {};
        virtual int  get_particle_id(double *pos) {};

        virtual std::vector<int> get_neighbour_list(int *ref_pos) {};
        virtual std::vector<int> get_neighbour_list(double *ref_pos) {};

        

};


}

#endif