#include "mxsrclib_dll_global.h"
#include "gpio_ftdi_control.h"
#include <memory>

static std::shared_ptr<gpio_ftdi_control_t> gpio_control;
using ftdi = gpio_ftdi_control_t;


MXSRCLIB_DLLEXPORT const char *get_channels_info() {
  return "none";
}

MXSRCLIB_DLLEXPORT void set_out_pins(uint32_t a_channel, uint32_t a_bus, uint8_t a_out_pins) {
  gpio_control->set_out_pins(static_cast<ftdi::channel_t>(a_channel), static_cast<ftdi::bus_t>(a_bus), a_out_pins);
}

MXSRCLIB_DLLEXPORT int write_gpio(uint32_t a_channel, uint32_t a_bus, uint8_t a_pins, int a_state) {
  return gpio_control->write_gpio(
    static_cast<ftdi::channel_t>(a_channel), static_cast<ftdi::bus_t>(a_bus), a_pins, a_state);
}

MXSRCLIB_DLLEXPORT int read_gpio(uint32_t a_channel, uint32_t a_bus, uint8_t a_pins) {
  return static_cast<int>(gpio_control->read_gpio(
    static_cast<ftdi::channel_t>(a_channel), static_cast<ftdi::bus_t>(a_bus), a_pins));
}

MXSRCLIB_DLLEXPORT void reinit() {
  gpio_control.reset();
  Sleep(2);
  gpio_control.reset(new gpio_ftdi_control_t());
}
