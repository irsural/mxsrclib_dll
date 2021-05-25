#include "multimeter_measure.h"
#include <irserror.h>
//#define debug_auto_volt_meas


const irs::string_t str_im_gpib = irst("gpib");
const irs::string_t str_im_usb = irst("usb");
const irs::string_t str_im_com = irst("com");

irs::string_t interface_multim_to_str(const interface_multim_t a_interface_mul)
{
  irs::string_t str_interface_multim;
  switch(a_interface_mul){
    case im_gpib: { str_interface_multim = str_im_gpib; } break;
    case im_usb:  { str_interface_multim = str_im_usb;  } break;
    case im_com:  { str_interface_multim = str_im_com;  } break;
  }
  return str_interface_multim;
}
interface_multim_t str_to_interface_multim(
  const irs::string_t& a_str_interface_multim)
{
  interface_multim_t interface_multim = im_gpib;
  if (a_str_interface_multim == str_im_gpib) {
    interface_multim = im_gpib;
  } else if (a_str_interface_multim == str_im_usb) {
    interface_multim = im_usb;
  } else if (a_str_interface_multim == str_im_com) {
    interface_multim = im_com;
  }
  return interface_multim;
}

const irs::string_t str_tmul_none_multimeter = irst("Не выбран");
const irs::string_t str_tmul_agilent_3458a = irst("Agilent 3458A");
const irs::string_t str_tmul_v7_78_1 = irst("В7-78\\1");
const irs::string_t str_tmul_ch3_85_3r = irst("Ч3-85\\3R");
const irs::string_t str_tmul_dummy = irst("Случайные числа");


irs::string_t type_multimetr_to_str(const type_multimetr_t a_type_multimetr)
{
  irs::string_t str_type_multimetr = str_tmul_none_multimeter;
  switch(a_type_multimetr){
    case tmul_agilent_3458a: {
      str_type_multimetr = str_tmul_agilent_3458a;
    } break;
    case tmul_v7_78_1: {
      str_type_multimetr = str_tmul_v7_78_1;
    } break;
    case tmul_ch3_85_3r: {
      str_type_multimetr = str_tmul_ch3_85_3r;
    } break;
    case tmul_dummy: {
      str_type_multimetr = str_tmul_dummy;
    } break;
    default :{
      str_type_multimetr = str_tmul_none_multimeter;
    }
  }
  return str_type_multimetr;
}

type_multimetr_t str_to_type_multimeter(
  const irs::string_t& a_str_type_multimetr)
{
  type_multimetr_t type_multimetr = tmul_none_multimeter;
  if (a_str_type_multimetr == str_tmul_agilent_3458a) {
    type_multimetr = tmul_agilent_3458a;
  } else if (a_str_type_multimetr == str_tmul_v7_78_1) {
    type_multimetr = tmul_v7_78_1;
  } else if (a_str_type_multimetr == str_tmul_ch3_85_3r) {
    type_multimetr = tmul_ch3_85_3r;
  } else if (a_str_type_multimetr == str_tmul_dummy) {
    type_multimetr = tmul_dummy;
  } else {
    type_multimetr = tmul_none_multimeter;
  }
  return type_multimetr;
}

//----------------------------------------------------
value_meas_t::value_meas_t():
  m_type_meas(tm_volt_dc),
  m_status_process(OFF_PROCESS),
  m_meas_status(meas_status_success),
  m_on_meas(false),
  m_on_wait_meas(false),
  m_on_connect_multimetr(false),
  m_num(0),
  m_denom(1),
  m_test_to(0),
  m_hardflow(NULL),
  m_multimeter(NULL),
  mp_value(NULL),
  m_config_commands()
{
  init_to_cnt();
}
value_meas_t::~value_meas_t()
{
  //m_multimeter.reset(NULL);
  m_multimeter = NULL;
  deinit_to_cnt();
}
void value_meas_t::set_time_wait_meas(int a_time)
{
  m_num = a_time;
}

void value_meas_t::process_meas()
{
  switch(m_status_process)
  {
    case OFF_PROCESS:{
      if(m_on_meas == true){
        m_on_meas = false;
        if (m_on_connect_multimetr) {
          m_status_process = WAIT_MEAS;
          set_to_cnt(m_test_to, TIME_TO_CNT(m_num, m_denom)); //задержка
          m_meas_status = meas_status_busy;
        } else {
          *mp_value = 0;
        }
      }
    } break;
    case WAIT_MEAS:{
      if(test_to_cnt(m_test_to)){
        #ifdef debug_auto_volt_meas
        #else
        if (m_type_meas == tm_value) {
          m_multimeter->get_value(mp_value);
        } else if(m_type_meas == tm_volt_dc) {
          m_multimeter->set_dc();
          m_multimeter->get_voltage(mp_value);
        } else if(m_type_meas == tm_volt_ac) {
          m_multimeter->set_ac();
          m_multimeter->get_voltage(mp_value);
        } else if(m_type_meas == tm_current_dc) {
          m_multimeter->set_dc();
          m_multimeter->get_current(mp_value);
        } else if(m_type_meas == tm_current_ac) {
          m_multimeter->set_ac();
          m_multimeter->get_current(mp_value);
        } else if(m_type_meas == tm_resistance_2x) {
          m_multimeter->get_resistance2x(mp_value);
        } else if(m_type_meas == tm_resistance_4x) {
          m_multimeter->get_resistance4x(mp_value);
        } else if(m_type_meas == tm_frequency) {
          m_multimeter->get_frequency(mp_value);
        } else if(m_type_meas == tm_phase) {
          m_multimeter->get_phase(mp_value);
        } else if(m_type_meas == tm_phase_average) {
          m_multimeter->get_phase_average(mp_value);
        } else if(m_type_meas == tm_time_interval) {
          m_multimeter->get_time_interval(mp_value);
        } else if(m_type_meas == tm_time_interval_average) {
         m_multimeter->get_time_interval_average(mp_value);
        } else{
          // сообщение об ошибке
        }
        #endif
        m_status_process = MEAS;
      }
    } break;
    case MEAS:{
      #ifdef debug_auto_volt_meas
      m_meas_status = meas_status_success;
      static int i = 0;
      if (i == INT_MAX)
        i = 0;
      voltage = i++;
      #else
      m_meas_status = m_multimeter->status();
      #endif
      if (m_meas_status == meas_status_success) {
        m_status_process = OFF_PROCESS;
      }
      else if (m_meas_status == meas_status_error) {
        m_status_process = OFF_PROCESS;
      }
    } break;
  }
}

void value_meas_t::set_extra_commands(const vector<irs::string> a_commands)
{
  if (m_on_connect_multimetr && (m_type_meas != tm_value)) {
    auto agilent = dynamic_cast<irs::agilent_3458a_t*>(m_multimeter);
    if (agilent) {
//      agilent->set_string_commands(a_commands);
    }
  }
}

void value_meas_t::tick()
{
  if(m_on_connect_multimetr) {
    m_multimeter->tick();
  }
  process_meas();
}
