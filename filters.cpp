#include "filters.h"


// class param_filter_t
param_filter_t::param_filter_t(double a_sampling_time,
  size_type a_num_of_points
):
  m_sample_timer(new irs::loop_timer_t(irs::make_cnt_s(a_sampling_time))),
  m_sko_calc(a_num_of_points),
  m_started(false),
  m_last_value(0)
{
}

void param_filter_t::set_sampling_time(double a_sampling_time)
{
  m_sample_timer.reset(new irs::loop_timer_t(irs::make_cnt_s(a_sampling_time)));
}

void param_filter_t::resize(size_type a_point_count)
{
  m_sko_calc.resize(a_point_count);
}

void param_filter_t::add(double a_value)
{
  m_last_value = a_value;
}

double param_filter_t::get_value() const
{
  return m_sko_calc.average();
}

void param_filter_t::restart()
{
  m_started = true;
  m_sko_calc.clear();
}

void param_filter_t::stop()
{
  m_started = false;
}

bool param_filter_t::started() const
{
  return m_started;
}

void param_filter_t::tick()
{
  if (m_sample_timer->check()) {
    if (m_started) {
      m_sko_calc.add(m_last_value);
    }
  }
}
