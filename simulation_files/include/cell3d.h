#include <iostream>
#include <cmath>
#include <cstdlib>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>
#include <vector>
#include <sstream>
#include <fstream>
#include <random>

#ifndef CELL_H
#define CELL_H

class cell
{
public:

    void
    initArrays(int N, int dim, int *L)
    {
        D = dim;
        L_total = 1;

        for (int axis = 0; axis < D; axis++)
            L_total *= L[axis];

        occupied_ = (int*)malloc(sizeof(int) * L_total);

        for (int i = 0; i < L_total; i++)
            occupied_[i] = N;

        L_eff = (int*)malloc(sizeof(int) * D);

        for (int axis = 0; axis < D; axis++)
            L_eff[axis] = 1;

        for (int axis = 0; axis < D; axis++)
        {
            for (int itr = axis+1; itr < D; itr++)
                L_eff[axis] *= L[itr];
        }
        
    }

    int
    occupied(int *location) const
    { 
        int counter=0;

        for (int axis = 0; axis < D; axis++)
            counter += location[axis] * L_eff[axis];
        
        return occupied_[counter]; 
    }

    int&
    occupied(int *location)
    {
        int counter=0;

        for (int axis = 0; axis < D; axis++)
            counter += location[axis] * L_eff[axis];
        
        return occupied_[counter]; 
    }


private:

    int  D;
    int  L_total;
    int *L_eff;
    int *occupied_;
    //int  counter;

};

#endif
