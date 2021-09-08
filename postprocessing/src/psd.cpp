#include <post.h>

namespace post_p{


void postprocessing::psd()
{
    double test_pos[D_];
    double centre_pos[D_];
    double delta_x[D_];
    double particle_pos[D_];
    std::vector<double> v;
    int id;
    double r;
    double r_max;
    double ref_r;
    double test_radius = 0.;
    double max_vertex_length;

    int iters = 0;
    int max_iters = 1e1;

    voro::container_poly *base_con = new voro::container_poly(lo_hi(0,0), lo_hi(0,1), lo_hi(1,0), lo_hi(1,1), lo_hi(2,0), lo_hi(2,1), 1, 1, 1, true, true, true, numParticles());

    for (int i = 0; i < numParticles(); i++)
        base_con->put(i, pos(i,0), pos(i,1), pos(i,2), radius(i));

    voro::c_loop_all    *vertex_loop = new voro::c_loop_all(*base_con);
    voro::voronoicell   *cell        = new voro::voronoicell;
    voro::c_loop_subset *base_loop   = new voro::c_loop_subset(*base_con);

    voro::container_poly    *sample_con;
    voro::c_loop_subset     *sample_loop;

    //base_con->compute_all_cells();

        max_vertex_length = 0.;

        if(vertex_loop->start()) do{
            base_con->compute_cell(*cell, *vertex_loop);
            vertex_loop->pos(particle_pos[0], particle_pos[1], particle_pos[2]);
            id=vertex_loop->pid();
            cell->vertices(particle_pos[0], particle_pos[1], particle_pos[2], v);

            for (int i = 0; i < cell->p; i++){

                for (int axis = 0; axis < dim(); axis++)
                    v[3*i+axis] = get_periodic_image(v[3*i+axis], axis);

                r = 0;
                
                for (int axis = 0; axis < dim(); axis++){
                    delta_x[axis]  = get_periodic_image(v[3*i+axis]-particle_pos[axis], axis);
                    r             += delta_x[axis] * delta_x[axis];
                }

                r  = sqrt(r);

                if (r > max_vertex_length){
                    max_vertex_length = r;
                }

            }


    }while(vertex_loop->inc());

    max_vertex_length += 1e-8;

    while (iters < max_iters){

        r_max = 0.;

        for (int axis = 0; axis < dim(); axis++)
            test_pos[axis] = 0.;

        base_con->find_voronoi_cell(test_pos[0], test_pos[1], test_pos[2], particle_pos[0], particle_pos[1], particle_pos[2], id);

        r = 0;
        
        for (int axis = 0; axis < dim(); axis++){
            delta_x[axis]  = get_periodic_image(test_pos[axis]-particle_pos[axis], axis);
            r             += delta_x[axis] * delta_x[axis];
        }

        r  = sqrt(r);

        if (r < radius(id))
            break;
        else
            iters++;

        base_loop->setup_sphere(test_pos[0], test_pos[1], test_pos[2], max_vertex_length, true);

        int base_i = 0;

        if(base_loop->start()) do  {

            std::cout<<"base = "<<base_i++<<std::endl;

            base_loop->pos(particle_pos[0], particle_pos[1], particle_pos[2]);
            id=base_loop->pid();
            cell->vertices(particle_pos[0], particle_pos[1], particle_pos[2], v);

            for (int i = 0; i < cell->p; i++){

                ref_r = 0;

                for (int axis = 0; axis < dim(); axis++)
                    v[3*i+axis] = get_periodic_image(v[3*i+axis], axis);

                for (int axis = 0; axis < dim(); axis++){
                    delta_x[axis] = get_periodic_image(v[3*i+axis]-test_pos[axis], axis);
                    ref_r        += (delta_x[axis] * delta_x[axis]);
                }
                
                ref_r = sqrt(ref_r);

                r = 0;
                
                for (int axis = 0; axis < dim(); axis++){
                    delta_x[axis]  = get_periodic_image(v[3*i+axis]-particle_pos[axis], axis);
                    r             += delta_x[axis] * delta_x[axis];
                }

                r  = sqrt(r);
                r -= radius(id);

                if ((ref_r < r) && (r > r_max)){
                    r_max = r;

                    for (int axis = 0; axis < dim(); axis++)
                        centre_pos[axis] = v[3*i+axis];

                }

            }


        }while(base_loop->inc());
        
        sample_con = new voro::container_poly(lo_hi(0,0), lo_hi(0,1), lo_hi(1,0), lo_hi(1,1), lo_hi(2,0), lo_hi(2,1), 1, 1, 1, true, true, true, numParticles()+1);

        for (int i = 0; i < numParticles(); i++)
            sample_con->put(i, pos(i,0), pos(i,1), pos(i,2),radius(i));

        sample_con->put(numParticles(), test_pos[0], test_pos[1], test_pos[2], test_radius);

        sample_loop = new voro::c_loop_subset(*sample_con);
        sample_loop->setup_sphere(test_pos[0], test_pos[1], test_pos[2], 1e-8);

        double vertex[D_];
        double rx[D_];

        int rid;

        int sample_i = 0;

        if(sample_loop->start()) do  {

            std::cout<<"sample i = "<<sample_i++<<std::endl;

            sample_loop->pos(particle_pos[0], particle_pos[1], particle_pos[2]);
            id=sample_loop->pid();

            if (id == numParticles()){

                sample_con->compute_cell(*cell,*sample_loop);
                cell->vertices(particle_pos[0], particle_pos[1], particle_pos[2], v);
                //std::cout<<"here"<<std::endl;

                for (int i = 0; i < cell->p; i++){

                    for (int axis = 0; axis < dim(); axis++)
                        vertex[axis] = v[3*i+axis];

                    for (int axis = 0; axis < dim(); axis++)
                        vertex[axis] = get_periodic_image(vertex[axis], axis);

                    base_con->find_voronoi_cell(vertex[0], vertex[1], vertex[2], rx[0], rx[1], rx[2], rid);                

                    r = 0;

                    for (int axis = 0; axis < dim(); axis++){
                        delta_x[axis] = get_periodic_image(vertex[axis]-rx[axis], axis);
                        r            += (delta_x[axis] * delta_x[axis]);
                    }

                    r  = sqrt(r);
                    r -= radius(rid);

                    ref_r = 0.;

                    for (int axis = 0; axis < dim(); axis++){
                        delta_x[axis] = get_periodic_image(vertex[axis]-particle_pos[axis], axis);
                        ref_r        += (delta_x[axis] * delta_x[axis]);
                    }

                    ref_r = sqrt(ref_r);

                    if ((r > r_max) && (ref_r < (r+1e-8))){

                        r_max = r;

                        for (int axis = 0; axis < dim(); axis++)
                            centre_pos[axis] = vertex[axis];
                        
                    }



                }

            }


        }while(sample_loop->inc());

        std::cout<<"largest radius = "<<r_max<<"\t with centre = "<<centre_pos[0]<<"\t"<<centre_pos[1]<<"\t"<<centre_pos[2]<<std::endl;

        delete sample_con;
        delete sample_loop;

    }

    //std::cout<<"radical test"<<std::endl;
    
    delete base_loop;
    delete base_con;
    delete cell;


}




}
