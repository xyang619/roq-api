/* Copyright (c) 2017-2020, Hans Erik Thrane */

#pragma once

#include <cstddef>
#include <stdexcept>

#include "roq/compat.h"

namespace roq {

//! A non-owning contiguous array of objects.
/*!
 * This is useful for providing access to raw data without leaking how data is
 * managed.
 *
 * The interface is a subset of \ref std::vector and
 * \ref std::array.
 *
 * The implementation closely follows the design of
 * `std::span <https://en.cppreference.com/w/cpp/container/span>`_ (since
 * C++20).
 */
template <typename T>
class span final {
 public:
  using value_type = T;  // XXX std::remove_cv_t<T>
  using reference = value_type&;
  using const_reference = const value_type&;
  using pointer = value_type*;
  using const_pointer = const value_type*;
  using iterator = pointer;

  span() noexcept
      : _array(nullptr),
        _length(0) {
  }

  span(
      pointer array,
      size_t length) noexcept
      : _array(array),
        _length(length) {
  }

  template <typename C>
  span(C& container)  // NOLINT
      : span(container.data(), container.size()) {
  }

  span(const span&) = default;
  span(span&&) = default;

  span& operator=(const span&) = default;
  span& operator=(span&&) = default;

  pointer data() {
    return _array;
  }
  const_pointer data() const {
    return _array;
  }

  bool empty() const {
    return _length == 0;
  }

  size_t size() const {
    return _length;
  }

  size_t size_bytes() const {
    return _length * sizeof(value_type);
  }

  reference operator[](size_t index) {
    return _array[index];
  }
  const_reference operator[](size_t index) const {
    return _array[index];
  }

  reference at(size_t index) {
    if (_length <= index)
      throw std::out_of_range();
    return _array[index];
  }
  const_reference at(size_t index) const {
    if (_length <= index)
      throw std::out_of_range();
    return _array[index];
  }

  iterator begin() const {
    return _array;
  }
  iterator end() const {
    return _array + _length;
  }
  iterator cbegin() const {
    return _array;
  }
  iterator cend() const {
    return _array + _length;
  }

  auto subspan(
      size_t offset,
      size_t count) const {
    // according to standard: undefined behavior if offset/count is out of range
    return span(_array + offset, count);
  }
  auto subspan(size_t offset) const {
    return subspan(offset, _length - offset);
  }

 private:
  pointer _array;
  size_t _length;
};

}  // namespace roq
