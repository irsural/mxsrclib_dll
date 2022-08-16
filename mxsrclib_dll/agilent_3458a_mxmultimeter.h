#ifndef AGILENT_3458A_MXMULTIMETER_T_H
#define AGILENT_3458A_MXMULTIMETER_T_H

#include <irsdevices.h>


class agilent_3458a_mxmultimeter_t: public irs::mxmultimeter_assembly_t
{
public:
  agilent_3458a_mxmultimeter_t(const wchar_t *a_name, int a_gpib_index, int a_gpib_address,
                               const wchar_t *a_com_name, const wchar_t *a_ip_str, const wchar_t *a_port_str);
  virtual bool enabled() const;
  virtual void enable(multimeter_mode_type_t a_mul_mode_type);
  virtual void disable();
  virtual mxmultimeter_t* mxmultimeter();
  virtual void tick();
  virtual void show_options();
private:
  void reset();
  irs::handle_t<irs::hardflow_t> make_gpib_hardflow(irs::hardflow::prologix_flow_t::end_line_t a_read_end_line,
    irs::hardflow::prologix_flow_t::end_line_t a_write_end_line, int a_prologix_timeout_m) ;

  typedef irs::mxdata_assembly_t::string_type string_type;
  string_type m_interface_name;
  int m_gpib_index;
  int m_gpib_address;
  string_type m_com_name;
  string_type m_ip_str;
  string_type m_port_str;

  bool m_enabled;
  multimeter_mode_type_t m_mul_mode_type;
  irs::handle_t<irs::hardflow_t> mp_hardflow;
  irs::handle_t<mxmultimeter_t> mp_multimeter;
};


#endif // AGILENT_3458A_MXMULTIMETER_T_H
