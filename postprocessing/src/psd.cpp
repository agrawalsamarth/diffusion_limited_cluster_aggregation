#include <post.h>

namespace post_p{


void postprocessing::psd()
{

    voro::container con(lo_hi(0,0), lo_hi(0,1), lo_hi(1,0), lo_hi(1,1), lo_hi(2,0), lo_hi(2,1), 1, 1, 1, true, true, true, 2*numParticles());

    /*std::cout<<lo_hi(0,0)<<" "<<lo_hi(0,1)<<" "<<lo_hi(1,0)<<" "<<lo_hi(1,1)<<" "<<lo_hi(2,0)<<" "<<lo_hi(2,1)<<std::endl;*/
    
    for (int i = 0; i < numParticles(); i++)
        con.put(i, pos(i,0), pos(i,1), pos(i,2));

    voro::c_loop_all cl(con);
    voro::voronoicell c;

    double x, y, z;
    int id;

    double ref_x, ref_y, ref_z;

    ref_x = 0.0;
    ref_y = 0.0;
    ref_z = 0.0;

    double delta_x;
    double delta_y;
    double delta_z;

    std::vector<double> v;

    double r;
    double r_max=0.;

    double ref_r;
    
    double c_x;
    double c_y;
    double c_z;

    double t_x;
    double t_y;
    double t_z;

    if(cl.start()) do  {
        cl.pos(x,y,z);
        id=cl.pid();

        con.compute_cell(c,cl);
        c.vertices(x,y,z,v);

        for (int i = 0; i < c.p; i++){

            delta_x  = get_periodic_image(v[3*i+0]-ref_x, 0);
            delta_y  = get_periodic_image(v[3*i+1]-ref_y, 1);
            delta_z  = get_periodic_image(v[3*i+2]-ref_z, 2);
            
            ref_r = (delta_x * delta_x) + (delta_y * delta_y) + (delta_z * delta_z);
            
            delta_x  = get_periodic_image(v[3*i+0] - x, 0);
            delta_y  = get_periodic_image(v[3*i+1] - y, 1);
            delta_z  = get_periodic_image(v[3*i+2] - z, 2);

            r = (delta_x * delta_x) + (delta_y * delta_y) + (delta_z * delta_z);

            //std::cout<<"v_x = "<<v[3*i+0]<<"\tv_y = "<<v[3*i+1]<<"\tv_z = "<<v[3*i+2]<<"\tr = "<<r<<std::endl;

            if ((ref_r < r) && (r > r_max)){
                r_max = r;
                c_x = v[3*i+0];
                c_y = v[3*i+1];
                c_z = v[3*i+2];
                t_x = x;
                t_y = y;
                t_z = z;
                //std::cout<<"1"<<std::endl;
            }


        }


    }while(cl.inc());

    
    voro::container con2(lo_hi(0,0), lo_hi(0,1), lo_hi(1,0), lo_hi(1,1), lo_hi(2,0), lo_hi(2,1), 1, 1, 1, true, true, true, 2 * numParticles());

    for (int i = 0; i < numParticles(); i++)
        con2.put(i, pos(i,0), pos(i,1), pos(i,2));

    con2.put(numParticles(), ref_x, ref_y, ref_z);

    voro::c_loop_all cl2(con2);
    //voro::voronoicell c2;

    double v_x;
    double v_y;
    double v_z;

    double rx;
    double ry;
    double rz;

    int rid;

    if(cl2.start()) do  {
        cl2.pos(x,y,z);
        id=cl2.pid();

        con2.compute_cell(c,cl2);
        c.vertices(x,y,z,v);

        if (id == numParticles()){

            std::cout<<"here"<<std::endl;

            for (int i = 0; i < c.p; i++){

                v_x = v[3*i+0];
                v_y = v[3*i+1];
                v_z = v[3*i+2];

                con.find_voronoi_cell(v_x, v_y, v_z, rx, ry, rz, rid);                

                delta_x  = get_periodic_image(v[3*i+0] - rx, 0);
                delta_y  = get_periodic_image(v[3*i+1] - ry, 1);
                delta_z  = get_periodic_image(v[3*i+2] - rz, 2);

                r = (delta_x * delta_x) + (delta_y * delta_y) + (delta_z * delta_z);

                //std::cout<<"r = "<<r<<"\tr_max = "<<r_max<<"\trid = "<<rid<<std::endl;
                //std::cout<<"v_x = "<<v_x<<"\tv_y = "<<v_y<<"\tv_z = "<<v_z<<std::endl;

                if (r > r_max){

                    r_max = r;
                    c_x = v_x;
                    c_y = v_y;
                    c_z = v_z;
                    std::cout<<"2"<<std::endl;

                }



            }

        }


    }while(cl2.inc());

    std::cout<<"largest radius = "<<sqrt(r_max)<<"\t with centre = "<<c_x<<"\t"<<c_y<<"\t"<<c_z<<std::endl;


}




}
