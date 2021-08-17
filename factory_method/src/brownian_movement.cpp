#include <brownian_movement.hh>

namespace simulation{


brownian_movement::brownian_movement(int dim, int rng_seed){

    D  = dim;
    dr = (int*)malloc(sizeof(int) * D);

    for (int axis = 0; axis < D; axis++)
        dr[axis] = 0;


    generator.seed(rng_seed);
    dis.param(std::uniform_real_distribution<double>::param_type(0.0, 1.0));
}

int* brownian_movement::delta_x()
{

    temp      = dis(generator);
    sign_rand = (temp > 0.5) - (temp <= 0.5); 
    axis_rand = (int)(dis(generator) * D);

    for (int axis = 0; axis < D; axis++){
        dr[axis] = (sign_rand * (axis == axis_rand));
        //std::cout<<"dr = "<<dr[axis]<<std::endl;
    }

    return dr;

}

double brownian_movement::get_rand()
{ return dis(generator); }


}