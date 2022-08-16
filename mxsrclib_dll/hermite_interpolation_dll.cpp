#include "mxsrclib_dll_global.h"
#include "hermit.h"
#include "multi_instance.h"

using pchip_factory_t = multi_instance_t<pchip_t<double>>;
pchip_factory_t m_pchip_factory;


MXSRCLIB_DLLEXPORT pchip_factory_t::instance_handle_t pchip_create() {
  return m_pchip_factory.create();
}

MXSRCLIB_DLLEXPORT void pchip_destroy(pchip_factory_t::instance_handle_t a_handle) {
  m_pchip_factory.destroy(a_handle);
}

MXSRCLIB_DLLEXPORT void pchip_set_points(pchip_factory_t::instance_handle_t a_handle,
                                         double *a_x_points, double *a_y_points, size_t a_size)
{
  m_pchip_factory.get(a_handle).set_points(a_x_points, a_y_points, a_size);
}

MXSRCLIB_DLLEXPORT double pchip_interpolate(pchip_factory_t::instance_handle_t a_handle, double a_x)
{
  return m_pchip_factory.get(a_handle)(a_x);
}
