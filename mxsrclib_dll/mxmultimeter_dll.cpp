#include "mxsrclib_dll_global.h"
#include "agilent_3458a_mxmultimeter.h"
#include "multimeter_measure.h"
#include "irsdevices.h"
#include <memory>


irs::handle_t<irs::mxmultimeter_assembly_t> mp_mxmultimeter_assembly;
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

MXSRCLIB_DLLEXPORT void multimeter_tick()
{
  if (m_meas_value.is_multimeter_connected()) {
    m_meas_value.tick();
  }
}

MXSRCLIB_DLLEXPORT int multimeter_get_status()
{
  meas_status_t status = meas_status_t::meas_status_error;
  if (m_meas_value.is_multimeter_connected()) {
    status = m_meas_value.get_status_meas();
  }
  return static_cast<int>(status);
}

MXSRCLIB_DLLEXPORT int multimeter_start_measure(size_t a_meas_type, double *a_value)
{
  int success = 0;
  if (a_meas_type >= tm_first && a_meas_type <= tm_last) {
    if (m_meas_value.get_status_meas() == meas_status_success){
      m_meas_value.execute_meas(static_cast<type_meas_t>(a_meas_type), a_value);
      success = 1;
    }
  }
  return success;
}

MXSRCLIB_DLLEXPORT void multimeter_set_range(size_t a_meas_type, double a_range)
{
  if (m_meas_value.is_multimeter_connected()) {
    m_meas_value.set_range(static_cast<type_meas_t>(a_meas_type), a_range);
  }
}

template<char delimiter>
class string_delimited_by : public irs::string
{};

template<char delimiter>
std::istream& operator>>(std::istream& is, string_delimited_by<delimiter>& output)
{
  std::getline(is, output, delimiter);
  return is;
}

MXSRCLIB_DLLEXPORT int multimeter_set_config(const char* a_config_string,
  double a_apply_delay_s)
{
  if (m_meas_value.is_multimeter_connected()) {
    istringstream input_stream(a_config_string);
    std::vector<irs::string> commands(
      (std::istream_iterator<string_delimited_by<';'>>(input_stream)),
      std::istream_iterator<string_delimited_by<';'>>());

    return m_meas_value.send_commands(std::move(commands), a_apply_delay_s);
  }
  return 0;
}
