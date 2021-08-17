#include <iostream>
#include <random>

#ifndef PARTICLE_MOVEMENT_H
#define PARTICLE_MOVEMENT_H

namespace simulation{

class particle_movement{

    public:

        int*    delta_x();
        double  get_rand();


};



}

#endif