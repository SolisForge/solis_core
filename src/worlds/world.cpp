#include "solis/world/world.hpp"

namespace solis::world {

Dimension::SharedPtr World::get_dimension(const char *name) const {
  if (auto it = dimensions.find(name); it != dimensions.end())
    return it->second;
  return nullptr;
}

} // namespace solis::world