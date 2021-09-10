#include <iostream>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>
#include <vector>
#include <sstream>
#include <fstream>
#include <random>
#include </home/samarth/codes/voronoi/include/voro++/voro++.hh>

#ifndef POSTPROCESSING_H
#define POSTPROCESSING_H

namespace post_p{

class postprocessing
{
    public:

    postprocessing(char *config_filename);
    ~postprocessing();
    void read_params();
    void read_config();
    void memory_allocation_function();

    std::vector<std::string> split_string_by_delimiter(const std::string& s, char delimiter);
    void read_params_parser(char *config_filename);
    void read_config_parser(char *config_filename);

    int dim() const;
    int numParticles() const;
    int maxAttachments() const;
    int totalClusters() const;
    double  lo_hi(const int axis, const int limit) const;
    double& lo_hi(const int axis, const int limit);
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
    int  original_seed(const int i) const;
    int& original_seed(const int i);
    int  current_seed(const int i) const;
    int& current_seed(const int i);
    double  diameter(const int i) const;
    double& diameter(const int i);
    double  radius(const int i) const;
    double& radius(const int i);
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
    int  delta_hist(const int i) const;
    int& delta_hist(const int i);
    int  cluster_percolation(const int i, const int axis) const;
    int& cluster_percolation(const int i, const int axis);


    double get_periodic_image(double x, const int axis);

    void print_positions();
    
    void calc_rij();
    void dump_rij_file();
    void dump_rij_file(char *filename);
    void dump_rij_hist_file(double bin_size);
    void dump_rij_hist_file(double bin_size, char *filename);
    void calc_rij_hist(double bin_size);
    void dump_unfolded_file();
    void dump_unfolded_file(char *filename);
    void dump_density_correlation(double bin_size);
    void dump_density_correlation(double bin_size, char *filename);
    void dump_percolation_file();
    void dump_percolation_file(char *filename);
    void dump_rog();
    void dump_rog(char *filename);
    void init_unfolding();
    void unfold(const int prev, const int next);
    void calc_unfolded_rij();
    void dump_unfolded_hist_file(double bin_size);
    void calc_unfolded_hist(double bin_size);
    void dump_scattering_function(double q_min, double q_max, int num_q);
    void dump_scattering_function(double q_min, double q_max, int num_q, char *filename);
    void calc_scattering_function(double q_min, double q_max, int num_q);
    void calc_density_correlation(double bin_size);
    void save_config();
    void save_unfolded_config();
    void save_unfolded_config(char *filename);
    void get_headers();
    bool check_percolation();
    double calc_rog();

    void psd(int total_iters);
    void save_radius_distribution();
    void save_radius_distribution(char *filename);

    private:

    std::mt19937 generator;
    std::uniform_real_distribution<double> dis;

    int     headers_;
    int     N_;
    int     D_;
    int     max_attachments_;
    int     folded_;
    int     lattice_;
    int     iters_;
    int     columns_;
    double  seed_mass_;
    double  alpha_;
    double  phi_;
    double *lo_hi_;
    double *L_;
    double *halfL_;
    double *diameter_;
    double *radius_;
    int    *periodic_;
    double *posDiff_;
    int    *posDiff_int_;
    double *pos_;
    int    *num_attachments_;
    int    *attachment_;
    int    *original_seed_;
    int    *current_seed_;
    double *r_ij_;
    double *r_ij_hist_;
    int    *rho_hist_;
    bool   *is_placed_;
    double *unfolded_coords_;
    int     temp_next;
    double *delta_coords_;
    double  q_min;
    double  q_max;
    double  dq;
    int     num_q;
    double *sq_;
    int    *unfolded_num_attachments_;
    int    *delta_hist_;
    int     max_delta_;
    int    *cluster_percolation_;
    int     totalClusters_;
    int     N_pairs_;
    int     r_ij_hist_bins_;
    
    bool    lattice_flag_ = false;
    bool    N_flag_ = false;
    bool    D_flag_ = false;
    bool    max_attachments_flag_ = false;
    bool    L_flag_ = false;
    bool    periodic_flag_ = false;
    bool    folded_flag_ = false;
    bool    iters_flag_ = false;
    bool    seed_mass_flag_ = false;
    bool    alpha_flag_ = false;
    bool    phi_flag_   = false;
    bool    columns_flag_ = false;

    double  *radius_dis;
    double  *centres;
    int     *type_dis;
    int      max_psd_iters;

};

}

#endif
