#include <post.h>

namespace post_p
{

void postprocessing::dump_voronoi_volume(char *filename)
{
    calculate_voronoi_volumes();

    FILE *f;

    f = fopen(filename, "w");
    fprintf(f, "id,V_vor\n");

    //std::cout<<numParticles()<<"\t"<<voronoi_volumes.size()<<std::endl;

    for (int i = 0; i < numParticles(); i++){
        fprintf(f, "%d,%lf\n", i, voronoi_volumes[i]);
    }


    fclose(f);
}

void postprocessing::calculate_voronoi_volumes()
{
    voro::container con(lo_hi(0,0), lo_hi(0,1), lo_hi(1,0), lo_hi(1,1), lo_hi(2,0), lo_hi(2,1), 1, 1, 1, true, true, true, numParticles());

    for (int i = 0; i < numParticles(); i++)
        con.put(i, pos(i,0), pos(i,1), pos(i,2));

    voro::c_loop_all cl(con);
    voro::voronoicell c;
    voronoi_volumes.resize(numParticles(), 0.0);
    int id;

    //std::cout<<lo_hi(0,0)<<" "<<lo_hi(0,1)<<" "<<lo_hi(1,0)<<" "<<lo_hi(1,1)<<" "<<lo_hi(2,0)<<" "<<lo_hi(2,1)<<std::endl;

    if(cl.start()) do if(con.compute_cell(c, cl)) {

        id=cl.pid();
        voronoi_volumes[id] = c.volume();
        //std::cout<<c.volume()<<std::endl;


    } while(cl.inc());


}


}