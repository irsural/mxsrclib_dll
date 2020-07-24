#include "mxsrclib_dll_global.h"
#include "irsfunnel.h"


irs::handle_t<irs::funnel_client_t> p_funnel_client;


MXSRCLIB_DLLEXPORT void funnel_client_create(irs::mxdata_t *a_mxdata, uint32_t a_funnel_start_index,
                                             uint32_t a_offset, uint32_t a_size)
{
  p_funnel_client.reset(new irs::funnel_client_t(a_mxdata, a_funnel_start_index, MS_TO_CNT(200),
                        a_offset, a_size, irs::fcm_reduce));
}

MXSRCLIB_DLLEXPORT void funnel_client_destroy() {
  p_funnel_client.reset(nullptr);
}

MXSRCLIB_DLLEXPORT int funnel_client_connected() {
  return p_funnel_client->connected();
}

MXSRCLIB_DLLEXPORT size_t funnel_client_get() {
  return reinterpret_cast<size_t>(p_funnel_client.get());
}

MXSRCLIB_DLLEXPORT void funnel_client_tick() {
  p_funnel_client->tick();
}

MXSRCLIB_DLLEXPORT void funnel_client_reset_is_read_complete() {
  p_funnel_client->reset_stat_read_complete();
}

MXSRCLIB_DLLEXPORT int is_read_complete() {
  return p_funnel_client->read_complete();
}

MXSRCLIB_DLLEXPORT int funnel_client_get_read_size() {
  return p_funnel_client->get_read_size();
}

MXSRCLIB_DLLEXPORT int funnel_client_get_write_size() {
  return p_funnel_client->get_write_size();
}
