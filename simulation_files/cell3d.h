#include <cmath>
#include <stdlib.h>
#include <iostream>

class cell3d
{
public:

    static const int D      = 3;
    const double rho        = 0.04;
    static const int L      = 100;
    const int N_eff         = (int)(1. * L * L * L * rho);
    static const int N      = 4096;


    cell3d()
    {
        occupied_ = (int*)malloc(sizeof(int) * L * L * L);
        L_eff     = L * L;

        initArrays();


    }

    void
    initArrays()
    {
        for (int i = 0; i < (L * L * L); i++)
            occupied_[i] = N;
    }

    int
    occupied(const int x, const int y, const int z) const
    { return occupied_[x * L_eff + y * L + z]; }
    int&
    occupied(const int x, const int y, const int z)
    { return occupied_[x * L_eff + y * L + z]; }


private:

    int *occupied_;
    int L_eff;

};
