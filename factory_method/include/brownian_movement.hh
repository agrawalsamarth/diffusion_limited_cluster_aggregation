#include <particle_movement.hh>

#ifndef BROWNIAN_MOVEMENT_H
#define BROWNIAN_MOVEMENT_H

namespace simulation{

class brownian_movement: public particle_movement{

    public:

        brownian_movement(int dim, int rng_seed);
        int*   delta_x();
        double get_rand();

    private:

        std::mt19937 generator;
        std::uniform_real_distribution<double> dis;
        int  D;
        int *dr;
        int  axis_rand;
        int  sign_rand;
        double temp;



};


}


#endif