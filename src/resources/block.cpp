#include "solis/resources/block.hpp"

namespace solis {

Block::Block(const char *pkg, const char *name)
    : package(pkg), resource_name(name) {}

} // namespace solis