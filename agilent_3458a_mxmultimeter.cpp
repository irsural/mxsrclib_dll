#include "agilent_3458a_mxmultimeter.h"
#include "niusbgpib_hardflow.h"


agilent_3458a_mxmultimeter_t::agilent_3458a_mxmultimeter_t(const wchar_t *a_name, int a_gpib_index, int a_gpib_address,
  const wchar_t *a_com_name, const wchar_t *a_ip_str, const wchar_t *a_port_str):
  m_interface_name(a_name),
  m_gpib_index(a_gpib_index),
  m_gpib_address(a_gpib_address),
  m_com_name(a_com_name),
  m_ip_str(a_ip_str),
  m_port_str(a_port_str),
  m_enabled(false),
  m_mul_mode_type(mul_mode_type_passive),
  mp_hardflow(),
  mp_multimeter()
{
}

bool agilent_3458a_mxmultimeter_t::enabled() const
{
  return m_enabled;
}

void agilent_3458a_mxmultimeter_t::enable(multimeter_mode_type_t a_mul_mode_type)
{
  if (m_enabled) {
    return;
  }
  m_mul_mode_type = a_mul_mode_type;
  reset();
}
void agilent_3458a_mxmultimeter_t::disable()
{
  mp_multimeter.reset();
  mp_hardflow.reset();
  m_enabled = false;
}

void agilent_3458a_mxmultimeter_t::reset()
{
  disable();
  using prologix_flow_type = irs::hardflow::prologix_flow_t;
  const prologix_flow_type::end_line_t read_end_line = prologix_flow_type::cr_lf;
  const prologix_flow_type::end_line_t write_end_line = prologix_flow_type::cr_lf;
  const int prologix_timeout_ms = 30;

  mp_hardflow = make_gpib_hardflow(read_end_line, write_end_line, prologix_timeout_ms);
  if (mp_hardflow.get() != nullptr) {
    mp_multimeter.reset(new irs::agilent_3458a_t(mp_hardflow.get(), m_mul_mode_type));
    if (mp_multimeter->status() != meas_status_t::meas_status_error) {
      m_enabled = true;
    }
  }
}

mxmultimeter_t* agilent_3458a_mxmultimeter_t::mxmultimeter()
{
  return mp_multimeter.get();
}

void agilent_3458a_mxmultimeter_t::tick()
{
  if (!mp_multimeter.is_empty()) {
    mp_hardflow->tick();
    mp_multimeter->tick();
  }
}

void agilent_3458a_mxmultimeter_t::show_options()
{
}

irs::handle_t<irs::hardflow_t> agilent_3458a_mxmultimeter_t::make_gpib_hardflow(
  irs::hardflow::prologix_flow_t::end_line_t a_read_end_line,
  irs::hardflow::prologix_flow_t::end_line_t a_write_end_line,
  int a_prologix_timeout_ms)
{
  irs::handle_t<irs::hardflow_t> gpib_hardflow;

  if (m_interface_name == irst("NI USB-GPIB")) {
    const string_type dll_name = get_ni_usb_gpib_dll_file_name(ni_fn_gpib_32);
    gpib_hardflow.reset(new irs::hardflow::ni_usb_gpib_flow_t(m_gpib_index, m_gpib_address, 1, 1, dll_name));

  } else if (m_interface_name == irst("Agilent USB-GPIB")) {
    const string_type dll_name = get_ni_usb_gpib_dll_file_name(ni_fn_agtgpib32);
    gpib_hardflow.reset(new irs::hardflow::ni_usb_gpib_flow_t(m_gpib_index, m_gpib_address, 1, 1, dll_name));

  } else if (m_interface_name == irst("Prologix COM-GPIB")) {
    gpib_hardflow.reset(new irs::hardflow::prologix_flow_t(new irs::hardflow::com_flow_t(
      m_com_name, CBR_4800, FALSE, NOPARITY, 8, ONESTOPBIT, DTR_CONTROL_DISABLE), m_gpib_address, a_read_end_line,
      a_write_end_line, a_prologix_timeout_ms));

  } else {
    mxip_t ip;
    irs_u16 port = 0;

    if (str_to_mxip(m_ip_str, &ip) && str_to_num(m_port_str, &port)) {
      gpib_hardflow.reset(new irs::hardflow::prologix_flow_t(new irs::hardflow::tcp_client_t(ip, port),
        m_gpib_address, a_read_end_line, a_write_end_line, a_prologix_timeout_ms));
    }
  }
  if (gpib_hardflow->is_channel_exists(irs::hardflow_t::invalid_channel)) {
    gpib_hardflow.reset(nullptr);
  }
  return gpib_hardflow;
}
