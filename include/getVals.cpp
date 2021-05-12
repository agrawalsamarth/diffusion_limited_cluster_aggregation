#include "post.h"

namespace post_p
{

  int
  postprocessing::dim() const
  { return D_; }

  int
  postprocessing::numParticles() const
  { return N_; }

  int
  postprocessing::maxAttachments() const
  { return max_attachments_; }
  
  double
  postprocessing::lo_hi(const int axis, const int limit) const
  { return lo_hi_[limit+2*axis]; }
  double&
  postprocessing::lo_hi(const int axis, const int limit)
  { return lo_hi_[limit+2*axis]; }

  double
  postprocessing::L(const int axis) const
  { return L_[axis]; }
  double&
  postprocessing::L(const int axis)
  { return L_[axis]; }

  double
  postprocessing::halfL(const int axis) const
  { return halfL_[axis]; }
  double&
  postprocessing::halfL(const int axis)
  { return halfL_[axis]; }

  int
  postprocessing::periodic(const int axis) const
  { return periodic_[axis]; }
  int&
  postprocessing::periodic(const int axis)
  { return periodic_[axis]; }

  double
  postprocessing::posDiff(const int axis) const
  { return posDiff_[axis]; }
  double&
  postprocessing::posDiff(const int axis)
  { return posDiff_[axis]; }

  double
  postprocessing::pos(const int i, const int axis) const
  { return pos_[axis+D_*i]; }
  double&
  postprocessing::pos(const int i, const int axis)
  { return pos_[axis+D_*i]; }

  int
  postprocessing::cluster_percolation(const int i, const int axis) const
  { return cluster_percolation_[axis+D_*i]; }
  int&
  postprocessing::cluster_percolation(const int i, const int axis)
  { return cluster_percolation_[axis+D_*i]; }

  int
  postprocessing::numAttachments(const int i) const
  { return num_attachments_[i]; }
  int&
  postprocessing::numAttachments(const int i)
  { return num_attachments_[i]; }

  int
  postprocessing::original_seed(const int i) const
  { return original_seed_[i]; }
  int&
  postprocessing::original_seed(const int i)
  { return original_seed_[i]; }

  int
  postprocessing::current_seed(const int i) const
  { return current_seed_[i]; }
  int&
  postprocessing::current_seed(const int i)
  { return current_seed_[i]; }

  double
  postprocessing::diameter(const int i) const
  { return diameter_[i]; }
  double&
  postprocessing::diameter(const int i)
  { return diameter_[i]; }

  int
  postprocessing::unfoldedAttachments(const int i) const
  { return unfolded_num_attachments_[i]; }
  int&
  postprocessing::unfoldedAttachments(const int i)
  { return unfolded_num_attachments_[i]; }

  int
  postprocessing::attachment(const int i, const int j) const
  { return attachment_[j+max_attachments_*i]; }
  int&
  postprocessing::attachment(const int i, const int j)
  { return attachment_[j+max_attachments_*i]; }

  bool
  postprocessing::is_placed(const int i) const
  { return is_placed_[i]; }
  bool&
  postprocessing::is_placed(const int i)
  { return is_placed_[i]; }

  double
  postprocessing::unfolded_coords(const int i, const int axis) const
  { return unfolded_coords_[axis+D_*i]; }
  double&
  postprocessing::unfolded_coords(const int i, const int axis)
  { return unfolded_coords_[axis+D_*i]; }
  
  double
  postprocessing::delta_coords(const int i, const int axis) const
  { return delta_coords_[axis+D_*i]; }
  double&
  postprocessing::delta_coords(const int i, const int axis)
  { return delta_coords_[axis+D_*i]; }

  int
  postprocessing::delta_hist(const int i) const
  { return delta_hist_[i]; }
  int&
  postprocessing::delta_hist(const int i)
  { return delta_hist_[i]; }

  double
  postprocessing::sq_unfolded(const int i) const
  { return sq_unfolded_[i]; }
  double&
  postprocessing::sq_unfolded(const int i)
  { return sq_unfolded_[i]; }

}
