#include "dlca_lattice_3d.h"

void dlca_lattice_3d::force_moves(char *move_file)
{

    std::ifstream parser(move_file, std::ifstream::in);
    int particle_id;
    
    std::string str;
    std::vector<std::string> results;

    while (getline(parser, str)){

        results = split_string_by_delimiter(str, ',');

        particle_id = stoi(results[0]);

        for (int axis = 0; axis < D; axis++)
            movements[axis] = stoi(results[1+axis]);

        for (int axis = 0; axis < D; axis++)
            location[axis] = pos(particle_id,axis);

        grid.occupied(location) = N;

        for (int axis = 0; axis < D; axis++)
            pos(particle_id, axis) = getPeriodicImage(pos(particle_id,axis)+movements[axis],axis);

        
        for (int axis = 0; axis < D; axis++)
            location[axis] = pos(particle_id,axis);

        grid.occupied(location) = particle_id;
        checkForAggregations(particle_id);

        reset_check_status();


    }


}