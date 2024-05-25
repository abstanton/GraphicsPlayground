#pragma once

namespace gp::math {

template <typename T>
T lerp(T curr, T target, float rate) {
  return curr + ((target - curr) * rate);
}

}