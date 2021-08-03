#include <iostream>

#ifndef BOUNDARY_CONDITIONS_H
#define BOUNDARY_CONDITIONS_H

namespace simulation{

class boundary_conditions{

    public:

        virtual int refill(int x, int L) {};
        virtual int refill(int old_pos, int new_pos, int L) {};

        virtual double refill(double x, double L) {};
        virtual double refill(double old_pos, double new_pos, double L) {};

};

}

#endif