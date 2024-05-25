#include "GPUBackend.h"

#include "gl/GlBackend.h"

namespace gp::gpu {
Backend* Backend::get() {
  static Backend* backend = new GlBackend;
  return backend;
}
}  // namespace gp::gpu