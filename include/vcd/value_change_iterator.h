#pragma once

#include <vcd/types.h>

#include <type_traits>

namespace vcd {

template <class T>
class value_change_iterator_base {
public:
  using value_type = T;
  using pointer = T *;
  using reference = T &;

  using container = std::conditional_t<
    std::is_const_v<T>,
    std::vector<std::decay_t<T>> const,
    std::vector<std::decay_t<T>>
  >;

  value_change_iterator_base (container & c, std::size_t index)
    : container_(&c)
    , index_(index)
  { }

  reference operator* () {
    return container_->at(index_);
  }

  pointer operator-> () {
    return &container_->at(index_);
  }

  value_change_iterator_base & operator++ () {
    ++index_;
    return *this;
  }

  value_change_iterator_base operator++ (int) {
    auto result = *this;
    ++index_;
    return result;
  }

  value_change_iterator_base & operator+= (int amount) {
    index_ += amount;
  }

  bool operator== (value_change_iterator_base const & other) const {
    return container_ == other.container_ and index_ == other.index_;
  }

private:

  container * container_;
  std::size_t index_;
};

template <class T>
value_change_iterator_base<T> & operator+ (value_change_iterator_base<T> const & it,
    int amount) {
  auto result = it;
  it += amount;
  return it;
}

using value_change_iterator = value_change_iterator_base<types::value_change>;
using const_value_change_iterator = value_change_iterator_base<types::value_change const>;

} // ns vcd
