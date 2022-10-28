/*
 * Copyright (C) 2022 TU Dresden
 * All rights reserved.
 *
 * Authors:
 *   Tassilo Tanneberger
 */

#ifndef REACTOR_CPP_MULTIPORT_HH
#define REACTOR_CPP_MULTIPORT_HH

#include <algorithm>
#include <atomic>
#include <iostream>
#include <type_traits>
#include <vector>

namespace reactor {
// struct which gets handed to the ports to they can talk back
// to the portbank
//
class BaseMultiport { // NOLINT cppcoreguidelines-special-member-functions,-warnings-as-errors
protected:
  std::atomic<std::size_t> size_{0};         // NOLINT
  std::vector<std::size_t> present_ports_{}; // NOLINT

public:
  BaseMultiport() = default;
  ~BaseMultiport() = default;

  // tells the parent multiport that this port has been set.
  [[nodiscard]] inline auto set_present(std::size_t index) -> bool {
    auto calculated_index = size_.fetch_add(1, std::memory_order_relaxed);

    // triggering hard error if calculated_index tries to set a port out that is not in the list
    reactor::reactor_assert(calculated_index < present_ports_.capacity());

    present_ports_[calculated_index] = index;
    return true;
  }

  // resets parent multiport
  inline void clear() noexcept {
    size_.store(0, std::memory_order_relaxed);
    present_ports_.clear();
  }
};

template <class T, class A = std::allocator<T>>
class Multiport : public BaseMultiport { // NOLINT cppcoreguidelines-special-member-functions
protected:
  std::vector<T> data_{}; //NOLINT cppcoreguidelines-non-private-member-variables-in-classes

public:
  using allocator_type = A;
  using value_type = typename A::value_type;
  using reference = typename A::reference;
  using const_reference = typename A::const_reference;
  using difference_type = typename A::difference_type;
  using size_type = typename A::size_type;

  using iterator = typename std::vector<T>::iterator;
  using const_iterator = typename std::vector<T>::const_iterator;

  Multiport() noexcept = default;
  ~Multiport() noexcept = default;

  auto operator==(const Multiport& other) const noexcept -> bool {
    return std::equal(std::begin(data_), std::end(data_), std::begin(other.data_), std::end(other.data_));
  }
  auto operator!=(const Multiport& other) const noexcept -> bool { return !(*this == other); };
  inline auto operator[](std::size_t index) noexcept -> T& { return data_[index]; }
  inline auto operator[](std::size_t index) const noexcept -> const T& { return data_[index]; }

  inline auto begin() noexcept -> iterator { return data_.begin(); };
  inline auto begin() const noexcept -> const_iterator { return data_.begin(); };
  inline auto cbegin() const noexcept -> const_iterator { return data_.cbegin(); };
  inline auto end() noexcept -> iterator { return data_.end(); };
  inline auto end() const noexcept -> const_iterator { return data_.end(); };
  inline auto cend() const noexcept -> const_iterator { return data_.cend(); };

  inline auto size() const noexcept -> size_type { return data_.size(); };
  [[nodiscard]] inline auto empty() const noexcept -> bool { return data_.empty(); };

  // present_indices_unsorted
  [[nodiscard]] inline auto present_indices_unsorted() const noexcept -> std::vector<std::size_t> {
    return std::vector<std::size_t>(std::begin(present_ports_), std::begin(present_ports_) + size_.load());
  }

  // present_indices_sorted
  [[nodiscard]] inline auto present_indices_sorted() const noexcept -> std::vector<std::size_t> {
    std::sort(std::begin(present_ports_), std::begin(present_ports_) + size_.load());
    return std::vector<std::size_t>(std::begin(present_ports_), std::begin(present_ports_) + size_.load());
  }

  // present_ports_sorted
  // present_ports_unsorted
};

template <class T, class A = std::allocator<T>> class ModifableMultiport : public Multiport<T, A> { //NOLINT
public:
  ModifableMultiport()
      : Multiport<T>() {} //NOLINT
  ~ModifableMultiport() = default;

  inline void reserve(std::size_t size) noexcept {
    this->data_.reserve(size);
    this->present_ports_.reserve(size);
  }

  inline void push_back(const T& elem) noexcept {
    this->data_.push_back(elem);
    this->present_ports_.emplace_back(0);
  }

  template <class... Args> inline void emplace_back(Args&&... args) noexcept {
    this->data_.emplace_back(args...);
    this->present_ports_.emplace_back(0);
  }
};
} // namespace reactor

#endif // REACTOR_CPP_MULTIPORT_HH
