#ifndef GPIO_FTDI_CONTROL_T_H
#define GPIO_FTDI_CONTROL_T_H

#include "libMPSSE_spi.h"

#include <vector>
#include <cstdint>
#include <map>


class gpio_ftdi_control_t
{
public:
  enum class channel_t {
    A = 0,
    B = 1
  };
  enum class bus_t {
    D = 0,
    C = 1
  };
  enum pin_t : uint8_t {
    pin_0 = 0b00000001,
    pin_1 = 0b00000010,
    pin_2 = 0b00000100,
    pin_3 = 0b00001000,
    pin_4 = 0b00010000,
    pin_5 = 0b00100000,
    pin_6 = 0b01000000,
    pin_7 = 0b10000000,
    all_pins = 0b11111111
  };
  enum class error_t {
    none = 0,
    cant_get_channel_info = 1,
    cant_get_num_channels = 2,
    cant_open_channel = 3,
    cant_init_channel = 4,
    cant_read_gpio_state = 5,
  };

  gpio_ftdi_control_t();
  ~gpio_ftdi_control_t();

  error_t get_last_error();
  const std::vector<FT_DEVICE_LIST_INFO_NODE> &get_channels_info() const noexcept;

  bool write_gpio(channel_t a_channel, bus_t a_bus, uint8_t a_pin, bool a_state);
  bool read_gpio(channel_t a_channel, bus_t a_bus, uint8_t a_pin);
  FT_STATUS read_byte(channel_t a_channel, bus_t a_bus, uint8_t *a_byte);
  FT_STATUS write_byte(channel_t a_channel, bus_t a_bus, uint8_t a_byte);
  void set_out_pins(channel_t a_channel, bus_t a_bus, uint8_t a_pins_direction);
private:
  enum cmd_t {
    set_data_lowbyte = 0x80,
    set_data_highbyte = 0x82,
    get_data_lowbyte = 0x81,
    get_data_highbyte = 0x83,
  };

  error_t m_error;
  std::vector<FT_DEVICE_LIST_INFO_NODE> m_dev_list;
  FT_HANDLE m_A_channel_handle;
  FT_HANDLE m_B_channel_handle;
  std::map<std::tuple<channel_t, bus_t>, uint8_t> m_pin_config;

  void fill_channels_info();
  void open_channel(channel_t a_channel, uint32_t a_channel_idx);
  uint8_t get_out_pins(channel_t a_channel, bus_t a_bus);
};

#endif // GPIO_FTDI_CONTROL_T_H
