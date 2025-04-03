#ifndef SOLIS_CHUNK_HPP
#define SOLIS_CHUNK_HPP

/**
  =================================== SOLIS ===================================

  This file contains the description of the needed types related to the worlds
  library.

  @author    Geoffrey Côte
  @date      30/03/25
  @version   0.0.1
  @copyright Meltwin - 2025
             Distributed under the MIT Licence
  =============================================================================
*/

#include "solis/resources/block.hpp"
#include <memory>

namespace solis::world {

// ============================================================================
//    World-related typedef
// ============================================================================

typedef uint8_t InChunkCoord_t;     /// Coordinate type inside of a chunk
typedef double WorldCoordinate_t;   /// Coordinate in the world
typedef int64_t ChunkCoordinate_t;  /// Coordinate type for the chunks
typedef int32_t RegionCoordinate_t; /// Coordinate type for the regions
typedef uint8_t LayerIndex;         /// Y-index integer coordinate

// ============================================================================
//    World-related constants
// ============================================================================
constexpr InChunkCoord_t CHUNK_SIZE{16};  /// Size of a chunk in blocks
constexpr uint8_t REGION_WIDTH_CHUNK{32}; /// Size of a region in chunk number
constexpr uint16_t REGION_WIDTH_BLOCK{
    REGION_WIDTH_CHUNK * CHUNK_SIZE}; /// Size of a region in blocks

} // namespace solis::world

#endif