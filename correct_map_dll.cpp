#include "mxsrclib_dll_global.h"
#include "correct_map.h"
#include <map>

std::map<uint32_t, correct_map_t*> correct_map_handles;


template<class T1, class T2>
uint32_t get_handle_number(const std::map<T1, T2> &a_handles_map)
{
  uint32_t first_free_number = 0;

  bool insert = false;
  for (auto [number, _]: a_handles_map) {
    // Первый хэндл всегда = 1
    first_free_number++;
    if (first_free_number < number) {
      insert = true;
      break;
    }
  }

  if (!insert) {
    first_free_number = a_handles_map.size() + 1;
  }

  return first_free_number;
}

MXSRCLIB_DLLEXPORT uint32_t correct_map_create() {
  correct_map_t *cm = new correct_map_t;

  uint32_t cm_handle = get_handle_number(correct_map_handles);
  correct_map_handles[cm_handle] = cm;
  return cm_handle;
}


MXSRCLIB_DLLEXPORT uint32_t correct_map_destroy(uint32_t a_handle) {
  correct_map_t *cm = correct_map_handles[a_handle];
  auto erased_count = correct_map_handles.erase(a_handle);
  if (erased_count) {
    delete cm;
  }
  return erased_count;
}


MXSRCLIB_DLLEXPORT void correct_map_set_x_points_count(uint32_t a_handle, uint32_t a_points_count)
{
  correct_map_t *cm = correct_map_handles[a_handle];
  if (cm) {
    cm->map_id = 1;
    cm->x_count = a_points_count;
  }
}


MXSRCLIB_DLLEXPORT void correct_map_set_y_points_count(uint32_t a_handle, uint32_t a_points_count)
{
  correct_map_t *cm = correct_map_handles[a_handle];
  if (cm) {
//    cm->map_id = 1;
    cm->y_count = a_points_count;
  }
}


MXSRCLIB_DLLEXPORT void correct_map_set_x_points(uint32_t a_handle, double* a_points, uint32_t a_points_count) {
  correct_map_t *cm = correct_map_handles[a_handle];
  if (cm) {
    for (uint32_t i = 0; i < a_points_count; ++i) {
      cm->x_points[i] = a_points[i];
    }
  }
}


MXSRCLIB_DLLEXPORT void correct_map_set_y_points(uint32_t a_handle, double* a_points, uint32_t a_points_count) {
  correct_map_t *cm = correct_map_handles[a_handle];
  if (cm) {
    for (uint32_t i = 0; i < a_points_count; ++i) {
      cm->y_points[i] = a_points[i];
    }
  }
}


MXSRCLIB_DLLEXPORT void correct_map_set_coef_points(uint32_t a_handle, double* a_points, uint32_t a_points_count) {
  correct_map_t *cm = correct_map_handles[a_handle];
  if (cm) {
    for (uint32_t i = 0; i < a_points_count; ++i) {
      cm->coef_points[i] = a_points[i];
    }
  }
}


MXSRCLIB_DLLEXPORT uint32_t correct_map_get_x_points_count(uint32_t a_handle)
{
  correct_map_t *cm = correct_map_handles[a_handle];
  uint32_t x_count = 0;
  if (cm) {
    x_count = cm->x_count;
  }
  return x_count;
}


MXSRCLIB_DLLEXPORT uint32_t correct_map_get_y_points_count(uint32_t a_handle)
{
  correct_map_t *cm = correct_map_handles[a_handle];
  uint32_t y_count = 0;
  if (cm) {
    y_count = cm->y_count;
  }
  return y_count;
}


MXSRCLIB_DLLEXPORT uint32_t correct_map_get_coef_points_count(uint32_t a_handle)
{
  correct_map_t *cm = correct_map_handles[a_handle];
  uint32_t coef_count = 0;
  if (cm) {
    coef_count = cm->x_count * cm->y_count;
  }
  return coef_count;
}

MXSRCLIB_DLLEXPORT int correct_map_get_x_points(uint32_t a_handle, double *a_points)
{
  correct_map_t *cm = correct_map_handles[a_handle];
  if (cm) {
    for (uint32_t i = 0; i < cm->x_count; ++i) {
      a_points[i] = cm->x_points[i];
    }
    return 1;
  } else {
    return 0;
  }
}


MXSRCLIB_DLLEXPORT int correct_map_get_y_points(uint32_t a_handle, double *a_points)
{
  correct_map_t *cm = correct_map_handles[a_handle];
  if (cm) {
    for (uint32_t i = 0; i < cm->y_count; ++i) {
      a_points[i] = cm->y_points[i];
    }
    return 1;
  } else {
    return 0;
  }
}


MXSRCLIB_DLLEXPORT int correct_map_get_coefs_points(uint32_t a_handle, double *a_points)
{
  correct_map_t *cm = correct_map_handles[a_handle];
  if (cm) {
    uint32_t coefs_count = cm->x_count * cm->y_count;
    for (uint32_t i = 0; i < coefs_count; ++i) {
      a_points[i] = cm->coef_points[i];
    }
    return 1;
  } else {
    return 0;
  }
}


MXSRCLIB_DLLEXPORT int correct_map_connect(uint32_t a_handle, size_t a_mxdata_addr)
{
  correct_map_t *cm = correct_map_handles[a_handle];
  if (cm) {
    irs::mxdata_t *mxdata = reinterpret_cast<irs::mxdata_t*>(a_mxdata_addr);
    int result = static_cast<int>(cm->connect(mxdata));
    return result;
  }
  return 0;
}
