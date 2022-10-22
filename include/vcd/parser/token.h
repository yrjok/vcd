#pragma once

#include <type_traits>

namespace vcd {

class token {
public:
  virtual ~token () = default;

  template <class T>
  bool is () const {
    static_assert(std::is_base_of_v<token, T>, "T must derive from token");
    return (dynamic_cast<T const *>(this) != nullptr);
  }

  template <class T>
  T & as () {
    static_assert(std::is_base_of_v<token, T>, "T must derive from token");
    return dynamic_cast<T &>(*this);
  }

  template <class T>
  T const & as () const {
    return const_cast<token &>(*this).as<T>();
  }
};

} // ns vcd
