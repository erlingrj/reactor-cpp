/*
 * Copyright (C) 2019 TU Dresden
 * All rights reserved.
 *
 * Authors:
 *   Christian Menard
 */

#include "../assert.hh"
#include "../environment.hh"

namespace reactor {

template <class T>
const std::set<Port<T>*>& Port<T>::typed_outward_bindings() const {
  // HACK this cast is ugly but should be safe as long as we only allow to
  // bind with Port<T>*. The alternative would be to copy the entire set and
  // cast each element individually, which is also ugly...
  return reinterpret_cast<const std::set<Port<T>*>&>(outward_bindings());
}

template <class T>
Port<T>* Port<T>::typed_inward_binding() const {
  // we can use a static cast here since we know that this port is always
  // connected with another Port<T>.
  return static_cast<Port<T>*>(inward_binding());
}

template <class T>
const Port<T>* Port<T>::typed_source() const {
  // we can use a static cast here since we know that this port is always
  // connected with another Port<T>.
  return static_cast<const Port<T>*>(source());
}

template <class T>
void Port<T>::set(const ImmutableValuePtr<T>& value_ptr) {
  VALIDATE(!has_inward_binding(),
           "set() may only be called on a ports that do not have an inward "
           "binding!");
  VALIDATE(value_ptr != nullptr, "Ports may not be set to nullptr!");
  auto scheduler = environment()->scheduler();
  this->value_ptr = std::move(value_ptr);
  scheduler->set_port(this);
}

template <class T>
const ImmutableValuePtr<T>& Port<T>::get() const {
  if (has_source()) {
    return typed_source()->get();
  } else {
    return value_ptr;
  }
}

template <class T>
bool Port<T>::is_present() const {
  if (has_source()) {
    return typed_source()->is_present();
  } else {
    return value_ptr != nullptr;
  }
}

}  // namespace reactor
