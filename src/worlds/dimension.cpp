#include "solis/world/dimension.hpp"
#include <algorithm>

namespace solis::world {

// ============================================================================
//    Constructor
// ============================================================================

Dimension::Dimension(DimType_t type, const char *name)
    : name(name), world_type(type) {}

// ============================================================================
//    Region methods
// ============================================================================
Region::SharedPtr
Dimension::get_region(const RegionCoordinate &coordinates) const {
  if (auto it = std::find_if(regions.begin(), regions.end(),
                             [&coordinates](const Region::SharedPtr r) {
                               return (r->coord.x == coordinates.x) &&
                                      (r->coord.z == coordinates.z);
                             });
      it != regions.end())
    return *it;
  return nullptr;
}

bool Dimension::is_region_loaded(const RegionCoordinate &coordinates) const {
  if (auto it = std::find_if(regions.begin(), regions.end(),
                             [&coordinates](const Region::SharedPtr r) {
                               return (r->coord.x == coordinates.x) &&
                                      (r->coord.z == coordinates.z);
                             });
      it != regions.end())
    return true;
  return false;
}

// ============================================================================
//    Chunk methods
// ============================================================================

Chunk::SharedPtr
Dimension::get_chunk(const ChunkCoordinate &coordinates) const {
  auto region = get_region(coordinates);
  if (region == nullptr)
    return nullptr;
  if (auto it = std::find_if(region->begin(), region->end(),
                             [&coordinates](const Chunk::SharedPtr c) {
                               return (c->coord.x == coordinates.x) &&
                                      (c->coord.z == coordinates.z);
                             });
      it != region->end())
    return *it;
  return nullptr;
}

bool Dimension::is_chunk_loaded(const ChunkCoordinate &coordinates) const {
  auto region = get_region(coordinates);
  if (region == nullptr)
    return false;
  if (auto it = std::find_if(region->begin(), region->end(),
                             [&coordinates](const Chunk::SharedPtr c) {
                               return (c->coord.x == coordinates.x) &&
                                      (c->coord.z == coordinates.z);
                             });
      it != region->end())
    return true;
  return false;
}

// ============================================================================
//    Data loading into the dimension
// ============================================================================

void Dimension::add_chunk(const Chunk::SharedPtr chunk) {
  if (!is_chunk_loaded<ChunkCoordinate>(chunk))
    return;
}

} // namespace solis::world