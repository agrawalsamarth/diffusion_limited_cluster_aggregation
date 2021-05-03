#include <iostream>
#include <cmath>
#include <cstdlib>

#ifndef POSTPROCESSING_H
#define POSTPROCESSING_H

namespace post_p{

class postprocessing
{
    public:

    postprocessing();
    ~postprocessing();
    void read_params();
    void read_config();

    int dim() const;
    int numParticles() const;
    int maxAttachments() const;
    double  L(const int axis) const;
    double& L(const int axis);
    double  halfL(const int axis) const;
    double& halfL(const int axis);
    int  periodic(const int axis) const;
    int& periodic(const int axis);
    double  posDiff(const int axis) const;
    double& posDiff(const int axis);
    double  pos(const int i, const int axis) const;
    double& pos(const int i, const int axis);
    int  numAttachments(const int i) const;
    int& numAttachments(const int i);
    int  unfoldedAttachments(const int i) const;
    int& unfoldedAttachments(const int i);
    int  attachment(const int i, const int j) const;
    int& attachment(const int i, const int j);
    bool  is_placed(const int i) const;
    bool& is_placed(const int i);
    double  unfolded_coords(const int i, const int axis) const;
    double& unfolded_coords(const int i, const int axis);
    double  delta_coords(const int i, const int axis) const;
    double& delta_coords(const int i, const int axis);
    double  sq_unfolded(const int i) const;
    double& sq_unfolded(const int i);
    

    double get_periodic_image(double x, const int axis);

    void print_positions();

    void dump_rij_file();
    void calc_rij();
    void dump_hist_file(double bin_size);
    void calc_hist(double bin_size, int num_bins);
    void dump_unfolded_file();
    void init_unfolding();
    void unfold(const int prev, const int next);
    void dump_delta_coords(int file_number);
    void calc_delta_coords();
    void calc_unfolded_rij();
    void dump_unfolded_hist_file(double bin_size);
    void calc_unfolded_hist(double bin_size);
    void dump_unfolded_scattering_function();
    void calc_unfolded_scattering_function();
    bool check_percolation();

    private:

    int     N_;
    int     D_;
    int     max_attachments_;
    double *L_;
    double *halfL_;
    int    *periodic_;
    double *posDiff_;
    double *pos_;
    int    *num_attachments_;
    int    *attachment_;
    double *r_ij_;
    double *hist_;
    bool   *is_placed_;
    double *unfolded_coords_;
    int     temp_next;
    double *delta_coords_;
    double  q_min;
    double  q_max;
    double  dq;
    int     num_q;
    double *sq_unfolded_;
    int    *unfolded_num_attachments_;
    //int    *unfolded_attachments;

    bool    r_ij_flag_ = false;
    bool    hist_flag_ = false;
    bool    is_placed_flag_ = false;
    bool    unfolded_coords_flag_ = false;

};

}

#include "init.cpp"
#include "getVals.cpp"
#include "read_params.cpp"
#include "read_config.cpp"
#include "periodic_image.cpp"
#include "calc_rij.cpp"
#include "unfolding.cpp"
#include "delta_coords.cpp"
#include "unfolded_hist.cpp"
#include "print_positions.cpp"
#include "calc_scattering.cpp"
#include "percolation.cpp"
#endif
