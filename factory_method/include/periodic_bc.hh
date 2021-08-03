#include "boundary_conditions.hh"

#ifndef PERIODIC_H
#define PERIODIC_H

namespace simulation{

class periodic_bc: public boundary_conditions{

    public:

        int    refill(int x, int L);
        double refill(double x, double L);


};


}

#endif