#ifndef FILTERS_H
#define FILTERS_H

#include <timer.h>
#include <irsalg.h>


class param_filter_t
{
public:
  typedef std::size_t size_type;
public:
  param_filter_t(double a_sampling_time = 0.1, size_type a_num_of_points = 100);
  void set_sampling_time(double a_sampling_time);
  void resize(size_type a_point_count);
  void add(double a_value);
  double get_value() const;
  void restart();
  void stop();
  bool started() const;
  void tick();
private:
  irs::handle_t<irs::loop_timer_t> m_sample_timer;
  irs::sko_calc_t<double, double> m_sko_calc;
  bool m_started;
  double m_last_value;
};

#endif // FILTERS_H
