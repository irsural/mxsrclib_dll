#include "gpio_ftdi_control.h"
#include <cassert>


gpio_ftdi_control_t::gpio_ftdi_control_t() :
  m_error(error_t::none),
  m_dev_list(),
  m_A_channel_handle(nullptr),
  m_B_channel_handle(nullptr),
  m_pin_config()
{
  Init_libMPSSE();
  fill_channels_info();
}

void gpio_ftdi_control_t::fill_channels_info()
{
  uint32 channels = 0;
  FT_STATUS status = SPI_GetNumChannels(&channels);

  if(channels > 0 && status == FT_OK) {
    m_dev_list.resize(channels);

    for(size_t i = 0; i < channels; ++i) {
      status = SPI_GetChannelInfo(i, &m_dev_list[i]);
      if (status != FT_OK) {
        m_error = error_t::cant_get_channel_info;
      }
    }
  } else {
    m_error = error_t::cant_get_num_channels;
  }

  for (auto &dev: m_dev_list) {
    if (strcmp(dev.SerialNumber, "A") == 0) {
      open_channel(channel_t::A);
    } else if (strcmp(dev.SerialNumber, "B") == 0) {
      open_channel(channel_t::B);
    } else {
      assert(false);
    }
  }
}

void gpio_ftdi_control_t::open_channel(channel_t a_channel)
{
  auto &m_channel_handle = a_channel == channel_t::A ? m_A_channel_handle : m_B_channel_handle;
  uint32_t channel_index = a_channel == channel_t::A ? 0 : 1;

  FT_STATUS status = SPI_OpenChannel(channel_index, &m_channel_handle);

  if (status == FT_OK) {

    ChannelConfig channelConf;
    channelConf.ClockRate = I2C_CLOCK_HIGH_SPEED_MODE;
    channelConf.LatencyTimer = 1;
    channelConf.configOptions = SPI_CONFIG_OPTION_MODE0 | SPI_CONFIG_OPTION_CS_DBUS3 | SPI_CONFIG_OPTION_CS_ACTIVELOW;
    channelConf.Pin = 0;

    status = SPI_InitChannel(m_channel_handle, &channelConf);

    if (status != FT_OK) {
      m_error = error_t::cant_init_channel;
    }
  } else {
    m_error = error_t::cant_open_channel;
  }
}

gpio_ftdi_control_t::~gpio_ftdi_control_t()
{
  if (m_A_channel_handle != nullptr) {
    SPI_CloseChannel(m_A_channel_handle);
  }
  if (m_B_channel_handle != nullptr) {
    SPI_CloseChannel(m_B_channel_handle);
  }
  Cleanup_libMPSSE();
}

bool gpio_ftdi_control_t::write_gpio(channel_t a_channel, bus_t a_bus, uint8_t a_pin, bool a_state)
{
  auto channel_handle = a_channel == channel_t::A ? m_A_channel_handle : m_B_channel_handle;
  cmd_set_gpio_line_t cmd_write = a_bus == bus_t::C ? cmd_set_gpio_line_t::set_highbyte : cmd_set_gpio_line_t::set_lowbite;
  uint8_t bit_mask = static_cast<uint32_t>(a_pin);

  uint8_t gpio_line_state = 0;
  FT_STATUS status = read_byte(a_channel, a_bus, &gpio_line_state);

  if (status == FT_OK) {
    uint8_t new_state = a_state ? gpio_line_state | bit_mask : gpio_line_state & ~bit_mask;
    status = FT_WriteGPIO(channel_handle, get_out_pins(a_channel, a_bus), new_state, cmd_write);
  }
  return status == FT_OK;
}

bool gpio_ftdi_control_t::read_gpio(channel_t a_channel, bus_t a_bus, uint8_t a_pin)
{
  auto channel_handle = a_channel == channel_t::A ? m_A_channel_handle : m_B_channel_handle;
  cmd_get_gpio_line_t cmd_read = a_bus == bus_t::C ? cmd_get_gpio_line_t::get_highbyte : cmd_get_gpio_line_t::get_lowbite;

  uint8_t gpio_line_state = 0;
  FT_ReadGPIO(channel_handle, &gpio_line_state, cmd_read);
  return gpio_line_state & static_cast<uint8_t>(a_pin);
}

FT_STATUS gpio_ftdi_control_t::read_byte(channel_t a_channel, bus_t a_bus, uint8_t *a_byte)
{
  auto channel_handle = a_channel == channel_t::A ? m_A_channel_handle : m_B_channel_handle;
  cmd_get_gpio_line_t cmd_read = a_bus == bus_t::C ? cmd_get_gpio_line_t::get_highbyte : cmd_get_gpio_line_t::get_lowbite;

  return FT_ReadGPIO(channel_handle, a_byte, cmd_read);
}

void gpio_ftdi_control_t::set_out_pins(channel_t a_channel, bus_t a_bus, uint8_t a_pins_direction)
{
  //a_pins_direction - каждый бит отвечает за направление соответствующего пина, 1 = out, 0 = in
  m_pin_config.insert(std::make_pair(std::make_tuple(a_channel, a_bus), a_pins_direction));
}

uint8_t gpio_ftdi_control_t::get_out_pins(channel_t a_channel, bus_t a_bus)
{
  return m_pin_config[std::make_tuple(a_channel, a_bus)];
}

const std::vector<FT_DEVICE_LIST_INFO_NODE> &gpio_ftdi_control_t::get_channels_info() const noexcept
{
  return m_dev_list;
}

gpio_ftdi_control_t::error_t gpio_ftdi_control_t::get_last_error()
{
  return m_error;
}
