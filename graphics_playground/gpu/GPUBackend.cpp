#include "GPUBackend.h"
#include "gl/GlBackend.h"

namespace gpu {
	Backend* Backend::get() {
		static Backend* backend = new GlBackend;
		return backend;
	}
}