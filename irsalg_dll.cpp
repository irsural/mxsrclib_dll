#include "mxsrclib_dll_global.h"
#include "revisions.h"
#include "irsalg.h"
#include "filters.h"

#include <limits>


param_filter_t param_filter;


MXSRCLIB_DLLEXPORT int revision() {
  return static_cast<int>(revisions::mscrclib_dll);
}


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


MXSRCLIB_DLLEXPORT void param_filter_tick()
{
  param_filter.tick();
}


MXSRCLIB_DLLEXPORT void param_filter_add(double a_value)
{
  param_filter.add(a_value);
}


MXSRCLIB_DLLEXPORT double param_filter_get_value()
{
  return param_filter.get_value();
}


MXSRCLIB_DLLEXPORT void param_filter_restart()
{
  param_filter.restart();
}


MXSRCLIB_DLLEXPORT void param_filter_set_sampling_time(double a_sampling_time)
{
  param_filter.set_sampling_time(a_sampling_time);
}


MXSRCLIB_DLLEXPORT void param_filter_resize(size_t a_new_size)
{
  param_filter.resize(a_new_size);
}


MXSRCLIB_DLLEXPORT void param_filter_stop()
{
  param_filter.stop();
}
