#include <post.h>

namespace post_p{


void postprocessing::psd(int total_iters)
{
    double test_pos[D_];
    double centre_pos[D_];
    double delta_x[D_];
    double particle_pos[D_];
    std::vector<double> v;
    std::vector<std::vector<double>> base_vertices(N_);
    int id;
    double r;
    double r_max;
    double ref_r;
    double test_radius = 0.;
    double max_vertex_length;
    double vertex[D_];
    double rx[D_];
    int rid;
    int size;
    
    radius_dis = (double*)malloc(sizeof(double) * total_iters);
    centres    = (double*)malloc(sizeof(double) * total_iters * 3);
    type_dis   = (int*)malloc(sizeof(int) * total_iters);


    int iters     = 0;
    max_psd_iters = total_iters;

    //voro::container_poly *base_con = new voro::container_poly(lo_hi(0,0), lo_hi(0,1), lo_hi(1,0), lo_hi(1,1), lo_hi(2,0), lo_hi(2,1), 1, 1, 1, true, true, true, numParticles());
    voro::container *base_con = new voro::container(lo_hi(0,0), lo_hi(0,1), lo_hi(1,0), lo_hi(1,1), lo_hi(2,0), lo_hi(2,1), 1, 1, 1, true, true, true, numParticles());


    //for (int i = 0; i < numParticles(); i++)
        //base_con->put(i, pos(i,0), pos(i,1), pos(i,2), radius(i));

    for (int i = 0; i < numParticles(); i++)
        base_con->put(i, pos(i,0), pos(i,1), pos(i,2));


    voro::c_loop_all    *vertex_loop = new voro::c_loop_all(*base_con);
    voro::c_loop_all    *base_loop   = new voro::c_loop_all(*base_con);
    voro::voronoicell   *cell        = new voro::voronoicell;

    //voro::container_poly    *sample_con;
    //voro::c_loop_all        *sample_loop;
    max_vertex_length = 0.;

    if(vertex_loop->start()) do{
        base_con->compute_cell(*cell, *vertex_loop);
        vertex_loop->pos(particle_pos[0], particle_pos[1], particle_pos[2]);
        id=vertex_loop->pid();
        cell->vertices(particle_pos[0], particle_pos[1], particle_pos[2], v);

        if (cell->p > 0)
            base_vertices[id] = v;

        for (int i = 0; i < cell->p; i++){

            for (int axis = 0; axis < dim(); axis++)
                vertex[axis] = get_periodic_image(v[3*i+axis], axis);

            r = 0;
            
            for (int axis = 0; axis < dim(); axis++){
                delta_x[axis]  = get_periodic_image(vertex[axis]-particle_pos[axis], axis);
                r             += delta_x[axis] * delta_x[axis];
            }

            r  = sqrt(r);

            if (r > max_vertex_length){
                max_vertex_length = r;
            }

        }


    }while(vertex_loop->inc());

    max_vertex_length += 1e-8;
    bool in_void_space;

    bool test;
    bool test_find;
    bool c_test;

    int find_type = 0;


    while (iters < max_psd_iters){

        r_max = 0.;
        test_find = true;

        for (int axis = 0; axis < dim(); axis++)
            //test_pos[axis] = 0.;
            test_pos[axis] = lo_hi(axis,0) + dis(generator) * (lo_hi(axis,1) - lo_hi(axis,0));

        //std::cout<<"test = "<<test_pos[0]<<" "<<test_pos[1]<<" "<<test_pos[2]<<std::endl;

        base_con->find_voronoi_cell(test_pos[0], test_pos[1], test_pos[2], particle_pos[0], particle_pos[1], particle_pos[2], id);

        r = 0;
        
        for (int axis = 0; axis < dim(); axis++){
            delta_x[axis]  = get_periodic_image(test_pos[axis]-particle_pos[axis], axis);
            r             += delta_x[axis] * delta_x[axis];
        }

        r  = sqrt(r);

        //std::cout<<"r="<<r<<std::endl;

        if (r < radius(id))
            in_void_space = false;
        else
            in_void_space = true;

        if (in_void_space){

            if(base_loop->start()){
            
                do{

                    base_loop->pos(particle_pos[0], particle_pos[1], particle_pos[2]);
                    id=base_loop->pid();
                    v.clear();
                    v = base_vertices[id];
                    size = v.size()/3;

                    r = 0;
                    
                    for (int axis = 0; axis < dim(); axis++){
                        delta_x[axis]  = get_periodic_image(test_pos[axis]-particle_pos[axis], axis);
                        r             += delta_x[axis] * delta_x[axis];
                    }

                    r  = sqrt(r);

                    if (r <= max_vertex_length){

                        for (int i = 0; i < size; i++){

                            ref_r = 0;

                            for (int axis = 0; axis < dim(); axis++)
                                vertex[axis] = get_periodic_image(v[3*i+axis], axis);

                            for (int axis = 0; axis < dim(); axis++){
                                delta_x[axis] = get_periodic_image(vertex[axis]-test_pos[axis], axis);
                                ref_r        += (delta_x[axis] * delta_x[axis]);
                            }
                            
                            ref_r = sqrt(ref_r);

                            r = 0;
                            
                            for (int axis = 0; axis < dim(); axis++){
                                delta_x[axis]  = get_periodic_image(vertex[axis]-particle_pos[axis], axis);
                                r             += delta_x[axis] * delta_x[axis];
                            }

                            r  = sqrt(r);
                            r -= radius(id);

                            /*if (iters == 1){
                                std::cout<<"r = "<<r<<"\t ref_r = "<<ref_r<<std::endl;
                            }*/

                            if ((ref_r < r) && (r > r_max)){
                                //std::cout<<"r_max old = "<<r_max<<std::endl;
                                r_max = r;
                                //std::cout<<"r_max new = "<<r_max<<std::endl;
                                test_find = false;
                                find_type = 1;

                                for (int axis = 0; axis < dim(); axis++)
                                    centre_pos[axis] = vertex[axis];

                            }

                        }

                    }


                }while(base_loop->inc());

            }
            
            /*sample_con = new voro::container_poly(lo_hi(0,0), lo_hi(0,1), lo_hi(1,0), lo_hi(1,1), lo_hi(2,0), lo_hi(2,1), 1, 1, 1, true, true, true, numParticles()+1);

            for (int i = 0; i < numParticles(); i++)
                sample_con->put(i, pos(i,0), pos(i,1), pos(i,2),radius(i));

            sample_con->put(numParticles(), test_pos[0], test_pos[1], test_pos[2], test_radius);
            sample_loop = new voro::c_loop_all(*sample_con);*/

            //if(sample_loop->start()) do  {

                //std::cout<<"sample i = "<<sample_i++<<std::endl;

                //sample_loop->pos(particle_pos[0], particle_pos[1], particle_pos[2]);
                //id=sample_loop->pid();

                //if (id == numParticles()){
                    
                    //c_test = base_con->compute_ghost_cell(cell, test_pos[0], test_pos[1], test_pos[2], r);
                    c_test = base_con->compute_ghost_cell(cell, test_pos[0], test_pos[1], test_pos[2]);

                    cell->vertices(particle_pos[0], particle_pos[1], particle_pos[2], v);

                    for (int i = 0; i < cell->p; i++){

                        for (int axis = 0; axis < dim(); axis++)
                            vertex[axis] = v[3*i+axis];

                        for (int axis = 0; axis < dim(); axis++)
                            vertex[axis] = get_periodic_image(vertex[axis], axis);

                        base_con->find_voronoi_cell(vertex[0], vertex[1], vertex[2], rx[0], rx[1], rx[2], rid);

                        for (int axis = 0; axis < dim(); axis++)
                            rx[axis] = get_periodic_image(rx[axis], axis);

                        ref_r = 0.;

                        for (int axis = 0; axis < dim(); axis++){
                            delta_x[axis] = get_periodic_image(vertex[axis]-particle_pos[axis], axis);
                            ref_r        += (delta_x[axis] * delta_x[axis]);
                        }

                        ref_r = sqrt(ref_r);             

                        r = 0;

                        for (int axis = 0; axis < dim(); axis++){
                            delta_x[axis] = get_periodic_image(vertex[axis]-rx[axis], axis);
                            r            += (delta_x[axis] * delta_x[axis]);
                        }

                        r  = sqrt(r);
                        r -= radius(rid);

                        if ((r > r_max) && (ref_r < (r+1e-8))){

                            r_max = r;

                            for (int axis = 0; axis < dim(); axis++)
                                centre_pos[axis] = vertex[axis];

                            test_find = false;
                            find_type = 2;
                            
                        }



                    }

                //}


            //}while(sample_loop->inc());

            //delete sample_con;
            //delete sample_loop;

            if (!test_find){
                radius_dis[iters] = r_max;
                type_dis[iters]   = find_type;

                for (int axis = 0; axis < dim(); axis++)
                    centres[iters*3+axis] = centre_pos[axis];


                iters++;
            }

        }

    }

    //std::cout<<"radical test"<<std::endl;
    
    delete base_loop;
    delete base_con;
    delete cell;


}

void postprocessing::save_radius_distribution()
{
    
    printf("max_radius,type,cx0,cx1,cx2\n");

    for (int i = 0; i < max_psd_iters; i++)
        printf("%lf,%d,%lf,%lf,%lf\n", radius_dis[i], type_dis[i], centres[i*3], centres[i*3+1], centres[i*3+2]);

    free(radius_dis);
    free(type_dis);
    free(centres);

}

void postprocessing::save_radius_distribution(char *filename)
{
    
    FILE *f;

    f = fopen(filename, "w");

    fprintf(f, "max_radius,type,cx0,cx1,cx2\n");

    for (int i = 0; i < max_psd_iters; i++)
        fprintf(f, "%lf,%d,%lf,%lf,%lf\n", radius_dis[i], type_dis[i], centres[i*3], centres[i*3+1], centres[i*3+2]);

    fclose(f);

    free(radius_dis);
    free(type_dis);
    free(centres);

}


}
