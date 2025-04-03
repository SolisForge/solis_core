#ifndef SOLIS_WORLD_CHUNK_HPP
#define SOLIS_WORLD_CHUNK_HPP

/**
  =================================== SOLIS ===================================

  This file contains the description of a chunk object to ensure
  format-agnostic world manipulation.

  @author    Geoffrey Côte
  @date      30/03/25
  @version   0.0.1
  @copyright Meltwin - 2025
             Distributed under the MIT Licence
  =============================================================================
*/

#include "solis/world/coordinates.hpp"
#include "solis/world/typedef.hpp"
#include <map>
#include <vector>

namespace solis::world {

typedef Block *Layer[CHUNK_SIZE][CHUNK_SIZE];

template <typename T> struct LocalizedStructure { T coord; };

/**
 * @brief
 *
 */
struct Chunk : std::map<LayerIndex, Layer>,
               LocalizedStructure<ChunkCoordinate> {
  typedef std::shared_ptr<Chunk> SharedPtr;
};

/**
 * @brief Definition of a MC region
 */
struct Region : std::vector<Chunk::SharedPtr>,
                LocalizedStructure<RegionCoordinate> {
  typedef std::shared_ptr<Region> SharedPtr;
};

} // namespace solis::world

#endif