#include "IProfile.hpp"

namespace MotionProfile {

class ConstantVelocityProfile : public IProfile {

  segment getSegment();
  double get_frequency();
  void initialize(double p_v);

private:
  double v;
};
} // namespace MotionProfile