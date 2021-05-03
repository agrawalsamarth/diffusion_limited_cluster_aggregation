#include "post.h"

namespace post_p
{
    void postprocessing::print_positions()
    {
        for (int i = 0; i < numParticles(); i++){
            for (int axis = 0; axis < dim(); axis++){

                if (axis == (dim()-1))
                    std::cout<<pos(i,axis)<<std::endl;
                else
                    std::cout<<pos(i,axis)<<"\t";

            }
        }        
    }

}