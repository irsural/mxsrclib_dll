#include "mxsrclib_dll_global.h"
#include "irsalg.h"
#include <limits>


MXSRCLIB_DLLEXPORT double student_t_inverse_distribution_2x(double a_confidence_level, uint32_t a_degrees_of_freedom) {
  irs::confidence_level_t confidence_level = irs::confidence_level_0_95;
  if (a_confidence_level == 0.95) {
    confidence_level = irs::confidence_level_0_95;
  } else if (a_confidence_level == 0.99) {
    confidence_level = irs::confidence_level_0_99;
  } else if (a_confidence_level == 0.999) {
    confidence_level = irs::confidence_level_0_999;
  } else {
    return std::numeric_limits<double>::quiet_NaN();
  }
  return irs::student_t_inverse_distribution_2x(confidence_level, a_degrees_of_freedom);
}

