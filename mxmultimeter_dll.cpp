#include "mxsrclib_dll_global.h"
#include "agilent_3458a_mxmultimeter.h"
#include "multimeter_measure.h"
#include "irsdevices.h"
#include <memory>


irs::handle_t<irs::mxmultimeter_assembly_t> mp_mxmultimeter_assembly;
irs::handle_t<mxmultimeter_t> mp_multimeter;
value_meas_t m_meas_value;


bool connect_to_multimeter(type_meas_t a_meas_type)
{
  bool success = false;
  if (a_meas_type >= tm_first && a_meas_type <= tm_last) {
    if (!mp_mxmultimeter_assembly.is_empty()) {
      multimeter_mode_type_t mode = a_meas_type == tm_value ? mul_mode_type_passive : mul_mode_type_active;
      mp_mxmultimeter_assembly->enable(mode);
      if (mp_mxmultimeter_assembly->enabled()) {
        if (m_meas_value.set_connect_multimeter(mp_mxmultimeter_assembly->mxmultimeter())) {
          success = true;
        }
      }
    }
  }
  return success;
}


MXSRCLIB_DLLEXPORT int connect_to_agilent_3458a(size_t a_meas_type, const wchar_t* a_name, int a_gpib_index,
  int a_gpib_address, const wchar_t* a_com_name, const wchar_t* a_ip_str, const wchar_t *a_port_str)
{
  mp_mxmultimeter_assembly.reset(nullptr);
  mp_mxmultimeter_assembly.reset(new agilent_3458a_mxmultimeter_t(a_name, a_gpib_index, a_gpib_address, a_com_name,
    a_ip_str, a_port_str));

  return static_cast<int>(connect_to_multimeter(static_cast<type_meas_t>(a_meas_type)));
}

MXSRCLIB_DLLEXPORT void disconnect_multimeter()
{
  m_meas_value.disconnect_multimeter();
}

MXSRCLIB_DLLEXPORT int multimeter_get_measured_value(size_t a_meas_type, double *a_value)
{
  int success = 0;
  if (m_meas_value.is_multimeter_connected()) {
    m_meas_value.tick();

    meas_status_t meas_stat = m_meas_value.get_status_meas();
    if (meas_stat == meas_status_success){
      if (a_meas_type >= tm_first && a_meas_type <= tm_last) {
        m_meas_value.execute_meas(static_cast<type_meas_t>(a_meas_type), a_value);
        success = 1;
      }
    }
  }
  return success;
}

MXSRCLIB_DLLEXPORT const wchar_t* multimeter_set_range(const wchar_t* a_name)
{
  return a_name;
}
