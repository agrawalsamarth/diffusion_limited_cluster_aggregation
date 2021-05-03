#include <cmath>
#include <stdlib.h>
#include <iostream>

class cell2d
{
public:

    static const int D      = 2;
    const double rho        = 0.04;
    static const int L      = 150;
    const int N_eff         = (int)(1. * L * L * L * rho);
    static const int N      = 4096;


    cell2d()
    {
        occupied_ = (int*)malloc(sizeof(int) * L * L);
        initArrays();


    }

    void
    initArrays()
    {
        for (int i = 0; i < (L * L); i++)
            occupied_[i] = N;
    }

    int
    occupied(const int x, const int y) const
    { return occupied_[x * L + y]; }
    int&
    occupied(const int x, const int y)
    { return occupied_[x * L + y]; }


private:

    int *occupied_;
    int L_eff;

};
