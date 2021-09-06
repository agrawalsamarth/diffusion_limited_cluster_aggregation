#include <post.h>

namespace post_p{


void postprocessing::psd()
{

    voro::container base_con(lo_hi(0,0), lo_hi(0,1), lo_hi(1,0), lo_hi(1,1), lo_hi(2,0), lo_hi(2,1), 1, 1, 1, true, true, true, 2*numParticles());

    /*for (int i = 0; i < numParticles(); i++)
        base_con.put(i, pos(i,0), pos(i,1), pos(i,2), radius(i));*/

    for (int i = 0; i < numParticles(); i++)
        base_con.put(i, pos(i,0), pos(i,1), pos(i,2));

    voro::c_loop_all  base_loop(base_con);
    voro::voronoicell cell;

    double test_pos[D_];
    double centre_pos[D_];
    double delta_x[D_];
    double particle_pos[D_];

    std::vector<double> v;

    int id;
    double r;
    double r_max=0.;

    double ref_r;

    test_pos[0] = 0.;
    test_pos[1] = 0.;
    test_pos[2] = 0.;

    if(base_loop.start()) do  {
        base_loop.pos(particle_pos[0], particle_pos[1], particle_pos[2]);
        id=base_loop.pid();

        /*for (int axis = 0; axis < dim(); axis++)
                std::cout<<particle_pos[axis]<<"\t";

        std::cout<<"\n";*/

        base_con.compute_cell(cell,base_loop);
        cell.vertices(particle_pos[0], particle_pos[1], particle_pos[2], v);

        for (int i = 0; i < cell.p; i++){

            ref_r = 0;

            /*for (int axis = 0; axis < dim(); axis++)
                std::cout<<v[3*i+axis]<<"\t";

            std::cout<<"\n";

            for (int axis = 0; axis < dim(); axis++)
                v[3*i+axis] = get_periodic_image(v[3*i+axis], axis);

            for (int axis = 0; axis < dim(); axis++)
                std::cout<<v[3*i+axis]<<"\t";

            std::cout<<"\n";*/

            for (int axis = 0; axis < dim(); axis++){
                delta_x[axis] = get_periodic_image(v[3*i+axis]-test_pos[axis], axis);
                ref_r        += (delta_x[axis] * delta_x[axis]);
            }
            
            r = 0;
            
            for (int axis = 0; axis < dim(); axis++){
                delta_x[axis]  = get_periodic_image(v[3*i+axis]-particle_pos[axis], axis);
                r             += delta_x[axis] * delta_x[axis];
            }

            //r -= radius(id);

            if ((ref_r < r) && (r > r_max)){
                r_max = r;

                for (int axis = 0; axis < dim(); axis++)
                    centre_pos[axis] = v[3*i+axis];

            }

        }


    }while(base_loop.inc());
    
    voro::container sample_con(lo_hi(0,0), lo_hi(0,1), lo_hi(1,0), lo_hi(1,1), lo_hi(2,0), lo_hi(2,1), 1, 1, 1, true, true, true, 2 * numParticles());

    /*for (int i = 0; i < numParticles(); i++)
        sample_con.put(i, pos(i,0), pos(i,1), pos(i,2),radius(i));

    sample_con.put(numParticles(), test_pos[0], test_pos[1], test_pos[2], 0.);*/

    for (int i = 0; i < numParticles(); i++)
        sample_con.put(i, pos(i,0), pos(i,1), pos(i,2));

    sample_con.put(numParticles(), test_pos[0], test_pos[1], test_pos[2]);


    voro::c_loop_all sample_loop(sample_con);


    double vertex[D_];
    double rx[D_];

    int rid;

    if(sample_loop.start()) do  {
        sample_loop.pos(particle_pos[0], particle_pos[1], particle_pos[2]);
        id=sample_loop.pid();

        sample_con.compute_cell(cell,sample_loop);
        cell.vertices(particle_pos[0], particle_pos[1], particle_pos[2], v);

        if (id == numParticles()){

            //std::cout<<"here"<<std::endl;

            for (int i = 0; i < cell.p; i++){

                for (int axis = 0; axis < dim(); axis++)
                    vertex[axis] = v[3*i+axis];

                base_con.find_voronoi_cell(vertex[0], vertex[1], vertex[2], rx[0], rx[1], rx[2], rid);                

                r = 0;

                for (int axis = 0; axis < dim(); axis++){
                    delta_x[axis] = get_periodic_image(vertex[axis]-rx[axis], axis);
                    r            += (delta_x[axis] * delta_x[axis]);
                }

                //r -= radius(rid);

                //std::cout<<"r = "<<r<<"\tr_max = "<<r_max<<"\trid = "<<rid<<std::endl;
                //std::cout<<"v_x = "<<v_x<<"\tv_y = "<<v_y<<"\tv_z = "<<v_z<<std::endl;

                if (r > r_max){

                    r_max = r;

                    for (int axis = 0; axis < dim(); axis++)
                        centre_pos[axis] = vertex[axis];
                    
                }



            }

        }


    }while(sample_loop.inc());

    std::cout<<"data structure test"<<std::endl;
    std::cout<<"largest radius = "<<sqrt(r_max)<<"\t with centre = "<<centre_pos[0]<<"\t"<<centre_pos[1]<<"\t"<<centre_pos[2]<<std::endl;


}




}
