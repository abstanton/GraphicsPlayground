#pragma once

namespace gp {

template <typename T>
class SingletonMixIn {
  friend T;

 public:
  static T& get() {
    static T obj;
    return obj;
  }
};

}  // namespace gp