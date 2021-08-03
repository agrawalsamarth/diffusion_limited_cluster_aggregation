#include <periodic_bc.hh>

namespace simulation{

int periodic_bc::refill(int x, int L){

    return (x + L * (x < 0) - L * (x >= L));

}

double periodic_bc::refill(double x, double L){

    return (x + L * (x < 0) - L * (x >= L));

}

}