#pragma once
#include <string>

namespace gp {

struct IResource {
  std::string path;
  unsigned int id = -1;
};

}  // namespace gp
