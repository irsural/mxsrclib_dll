#include "mxsrclib_dll_global.h"
#include "gpio_ftdi_control.h"
#include <memory>

using ftdi = gpio_ftdi_control_t;

std::shared_ptr<gpio_ftdi_control_t>& get_gpio_control() {
  static std::shared_ptr<gpio_ftdi_control_t> gpio_control;
  return gpio_control;
}


MXSRCLIB_DLLEXPORT void ftdi_set_out_pins(uint32_t a_channel, uint32_t a_bus, uint8_t a_out_pins)
{
  if (get_gpio_control().get() != nullptr) {
    get_gpio_control()->set_out_pins(static_cast<ftdi::channel_t>(a_channel), static_cast<ftdi::bus_t>(a_bus), a_out_pins);
  }
}

MXSRCLIB_DLLEXPORT int ftdi_write_gpio(uint32_t a_channel, uint32_t a_bus, uint8_t a_pins, int a_state) {
  int success = 0;
  if (get_gpio_control().get() != nullptr) {
    success = get_gpio_control()->write_gpio(static_cast<ftdi::channel_t>(a_channel), static_cast<ftdi::bus_t>(a_bus), a_pins, a_state);
  }
  return success;
}

MXSRCLIB_DLLEXPORT int ftdi_read_gpio(uint32_t a_channel, uint32_t a_bus, uint8_t a_pins) {
  int success = 0;
  if (get_gpio_control().get() != nullptr) {
    success =
        get_gpio_control()->read_gpio(static_cast<ftdi::channel_t>(a_channel), static_cast<ftdi::bus_t>(a_bus), a_pins) == FT_OK;
  }
  return success;
}

MXSRCLIB_DLLEXPORT int ftdi_write_byte(uint32_t a_channel, uint32_t a_bus, uint8_t a_byte) {
  int success = 0;
  if (get_gpio_control().get() != nullptr) {
    success =
        get_gpio_control()->write_byte(static_cast<ftdi::channel_t>(a_channel), static_cast<ftdi::bus_t>(a_bus), a_byte) == FT_OK;
  }
  return success;
}

MXSRCLIB_DLLEXPORT int ftdi_reinit()
{
  get_gpio_control().reset();
  Sleep(2);
  get_gpio_control().reset(new gpio_ftdi_control_t());
  return static_cast<int>(get_gpio_control()->get_last_error() == gpio_ftdi_control_t::error_t::none);
}
