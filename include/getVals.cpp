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
  postprocessing::numAttachments(const int i) const
  { return num_attachments_[i]; }
  int&
  postprocessing::numAttachments(const int i)
  { return num_attachments_[i]; }

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

  double
  postprocessing::sq_unfolded(const int i) const
  { return sq_unfolded_[i]; }
  double&
  postprocessing::sq_unfolded(const int i)
  { return sq_unfolded_[i]; }

}
