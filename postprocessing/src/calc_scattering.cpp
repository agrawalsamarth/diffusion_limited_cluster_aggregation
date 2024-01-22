#include <post.h>

namespace post_p{

void postprocessing::dump_scattering_function(double q_min, double q_max, int num_q, char *filename)
{
    calc_scattering_function(q_min, q_max, num_q);

    double dq = log(q_max/q_min)/(1 * num_q);

    FILE *f;
    f= fopen(filename,"w");

    fprintf(f, "q,S(q)\n");

    for (int i = 0; i < num_q; i++)
        fprintf(f, "%lf,%lf\n", exp((1.*i*dq)+log(q_min)), 2. * sq_[i]);

    fclose(f);
    free(sq_);

}

void postprocessing::dump_scattering_function(double q_min, double q_max, int num_q)
{
    calc_scattering_function(q_min, q_max, num_q);

    double dq = log(q_max/q_min)/(1 * num_q);

    printf("q,S(q)\n");

    for (int i = 0; i < num_q; i++)
        printf("%lf,%lf\n", exp((1.*i*dq)+log(q_min)), 2. * sq_[i]);

    free(sq_);

}

void postprocessing::calc_scattering_function(double q_min, double q_max, int num_q)
{
    calc_rij();

    double N_inv = 1./(1.* numParticles());
    double q;
    double dq = log(q_max/q_min)/(1 * num_q);

    sq_ = (double*)malloc(sizeof(double)*num_q);

    for (int i = 0; i < num_q; i++)
        sq_[i] = 0.5;

    for (int i = 0; i < num_q; i++){

        q = exp((1.*i*dq)+log(q_min));

        for (int j = 0; j < N_pairs_; j++){
            sq_[i] += (N_inv * ((sin(q*r_ij_[j]))/(q*r_ij_[j])));
        }

    }

}

void postprocessing::bulltest(int num)
{
    return;
}

void postprocessing::calc_long_range_iq(int dr_steps, double q_min, double q_max, int num_q, char *filename)
{
    calc_rij();

    /*double dr    = (0.5*L(0))/(1.*dr_steps);
    double r_min = 0.5*dr;
    double r_max = L(0)+0.5*dr;*/

    double dr;
    double r_min=0.;
    double r_max=(0.5*L(0));    

    dr = (r_max-r_min)/(1.*dr_steps);
    int steps = (int)((r_max-r_min)/(1.*dr));    
    rdf       = (double*)malloc(sizeof(double)*steps);

    int index;

    for (int i = 0; i < steps; i++)
        rdf[i] = 0.;

    for (int i = 0; i < N_pairs_; i++){

        if (r_ij_[i] < r_max){
            index = (int)((r_ij_[i] - r_min)/dr);
            rdf[index] += 1.;
        }

    }

    double prefactor;
    double r_val;

    for (int i = 0; i < steps; i++){
        r_val = r_min+(1.*i*dr)+0.5*dr; 
        prefactor = 12.*phi_*r_val*r_val*dr*numParticles();
        rdf[i] /= prefactor;
    }

    double q;
    double sq_lr_temp;
    double dq = log(q_max/q_min)/(1 * num_q);

    sq_ = (double*)malloc(sizeof(double)*num_q);

    double g0;
    double num_val = 0.;
    double denom_val = 0.;

    for (int i = 0; i < steps; i++){
        r_val      = r_min+(1.*i*dr)+0.5*dr; 
        num_val   += (rdf[i]*4.*M_PI*r_val*r_val*dr);
        denom_val += (4.*M_PI*r_val*r_val*dr);
    }

    g0 = (num_val + (M_PI/(6.*phi_)))/denom_val;
    double form_factor;

    for (int i = 0; i < num_q; i++)
    {
        q = exp(0.5*dq+(1.*i*dq)+log(q_min));
        sq_lr_temp = 0.;
        prefactor  = (6.*phi_)/M_PI;

        for (int j = 0; j < steps; j++) {
            r_val = r_min+(1.*j*dr)+0.5*dr;
            sq_lr_temp += ((4.*M_PI*r_val*r_val*dr*(rdf[j]-g0)*sin(q*r_val))/(q*r_val)); 
        }

        sq_[i]  = 1. + (prefactor*sq_lr_temp);
        
        form_factor = (24. * (sin(0.5*q) - ((0.5*q)*cos(0.5*q))))/(q*q*q);
        sq_[i] *= (form_factor*form_factor);
    }



    FILE *f;
    f = fopen(filename, "w");

    fprintf(f,"q,Sq\n");

    for (int i = 0; i < num_q; i++)
        fprintf(f, "%lf,%lf\n", exp((1.*i*dq)+log(q_min)), sq_[i]);

    //fprintf(f,"r,g(r)\n");

    //for (int i = 0; i < steps; i++)
        //fprintf(f, "%lf,%lf\n", r_min+(1.*i*dr)+0.5*dr, rdf[i]);

    //for (int i = 0; i < N_pairs_; i++)
        //fprintf(f, "%lf\n", r_ij_[i]);


    fclose(f);
    free(sq_);
    free(rdf);


}

}