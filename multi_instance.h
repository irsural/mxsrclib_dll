#ifndef MULTI_INSTANCE_H
#define MULTI_INSTANCE_H

#include <cassert>
#include <map>

// Класс для создания экземпляров других классов и обращения к этим экземплярам через целочисленные дескрипторы
// Нужен для проброса нескольких инстансов одного класса через dll
template <class T>
class multi_instance_t
{
public:
  static_assert(std::is_move_constructible_v<T>, "multi_instance inner type must have move constructor");
  using instance_handle_t = size_t;

  multi_instance_t();
  ~multi_instance_t();

  template <class... T_args>
  instance_handle_t create(T_args&&... a_T_args)
  {
    instance_handle_t handle = get_instance_handle();
    m_instance_map.emplace(handle, T(std::forward<T_args>(a_T_args)...));
    return handle;
  }
  void destroy(instance_handle_t a_instance_handle);

  T& get(instance_handle_t a_handle);

private:
  instance_handle_t get_instance_handle();

  std::map<instance_handle_t, T> m_instance_map;
};

template <class T>
multi_instance_t<T>::multi_instance_t() :
  m_instance_map()
{

}

template <class T>
multi_instance_t<T>::~multi_instance_t()
{

}

template <class T>
void multi_instance_t<T>::destroy(instance_handle_t a_instance_handle)
{
  m_instance_map.erase(a_instance_handle);
}

template<class T>
T& multi_instance_t<T>::get(instance_handle_t a_handle)
{
  return m_instance_map[a_handle];
}

template<class T>
typename multi_instance_t<T>::instance_handle_t multi_instance_t<T>::get_instance_handle()
{
  uint32_t first_free_number = 0;

  bool insert = false;
  for (const auto& [number, _]: m_instance_map) {
    // Первый хэндл всегда = 1
    first_free_number++;
    if (first_free_number < number) {
      insert = true;
      break;
    }
  }

  if (!insert) {
    first_free_number = m_instance_map.size() + 1;
  }

  return first_free_number;
}

#endif // MULTI_INSTANCE_H
