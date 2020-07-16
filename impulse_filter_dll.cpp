#include "mxsrclib_dll_global.h"
#include "imp_filt.h"

static irs::filt_imp_noise_t impulse_filter;
static vector<irs::filt_imp_noise_t::value_type> filter_data;



MXSRCLIB_DLLEXPORT void imp_filter_clear() {
  impulse_filter.clear();
}

MXSRCLIB_DLLEXPORT irs::filt_imp_noise_t::value_type imp_filter_get() {
  return impulse_filter.get();
}

MXSRCLIB_DLLEXPORT void imp_filter_assign(irs::filt_imp_noise_t::value_type *a_values, uint32_t a_size) {
  vector<irs::filt_imp_noise_t::value_type> values(a_values, a_values + a_size);
  impulse_filter.assign(values);
}
