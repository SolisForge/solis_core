#ifndef SOLIS_UTILS_FLAGS_HPP
#define SOLIS_UTILS_FLAGS_HPP

/**
  =================================== SOLIS ===================================

  This file contains methods relative to coordinates.

  @author    Geoffrey Côte
  @date      30/03/25
  @version   0.0.1
  @copyright Meltwin - 2025
             Distributed under the MIT Licence
  =============================================================================
*/

#include "solis/world/typedef.hpp"
#include <cmath>

namespace solis::world {

// ============================================================================
//    Coordinate systems
// ============================================================================

/**
 * @brief Coordinate interface to implement as many coordinate systems as
 * needed.
 */
struct CoordinateInterface {};
// template <typename C> struct _Wrapper {
//   C coord;

//   constexpr _Wrapper(const C &c) : coord(c) {
//     static_assert(!std::is_base_of<CoordinateInterface, C>::value,
//                   "Coordinate wrapper type should be of
//                   CoordinateInterface.");
//   }
//   constexpr operator C &() { return coord; }
// };
/**
 * @brief Template coordinate type in 3D space.
 */
template <typename T> struct Coordinate3D {
  T x, y, z;
  constexpr Coordinate3D<T>() : x(0), y(0), z(0) {}
  constexpr Coordinate3D<T>(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}
};
/**
 * @brief Template coordinate type in 2D space.
 *
 * @tparam T
 */
template <typename T> struct Coordinate2D {
  T x, z;
  constexpr Coordinate2D<T>() : x(0), z(0) {}
  constexpr Coordinate2D<T>(T _x, T _z) : x(_x), z(_z) {}
};

//    Implementation of coordinates types
// ----------------------------------------------------------------------------

/**
 * @brief Coordinate in the world (so in float values).
 * The Y axis is up.
 */
typedef Coordinate3D<WorldCoordinate_t> WorldCoordinate;
/**
 * @brief Coordinates values of a chunk.
 * It is represented in 2D (X-Z plane) integer values.
 */
typedef Coordinate2D<ChunkCoordinate_t> ChunkCoordinate;
/**
 * @brief Coordinates values of a region.
 * It is represented in 2D (X-Z plane) integer values.
 */
typedef Coordinate2D<RegionCoordinate_t> RegionCoordinate;

// ============================================================================
//    Coordinate conversion system
// ============================================================================

/**
 * @brief Convert a coordinate system to another one
 *
 * @tparam The output type
 * @tparam The input type
 * @param c_in the input coordinates
 * @return the converted coordinates
 */
template <typename Tout, typename Tin>
inline constexpr Tout cvtCoordinate([[__maybe_unused__]] const Tin &c_in) {
  // Static assert to prevent using it without the proper methods
  static_assert(!std::is_base_of<CoordinateInterface, Tin>::value,
                "The coordinate conversion function should be called with a "
                "CoordinateInterface type");
  static_assert(!std::is_base_of<CoordinateInterface, Tout>::value,
                "The coordinate conversion function should be called with a "
                "CoordinateInterface type");
  // Unimplemented methods error raising
  static_assert(std::is_base_of<CoordinateInterface, Tin>::value,
                "Unimplemented coordinate conversion function");
  static_assert(std::is_base_of<CoordinateInterface, Tout>::value,
                "Unimplemented coordinate conversion function");
}

#define CVT_COORDINATE_HEADER(Tin, Tout)                                       \
  template <>                                                                  \
  inline constexpr Tout cvtCoordinate<Tout>(                                   \
      [[__maybe_unused__]] const Tin &c_in)

//    Implementation of coordinates conversion function
// ----------------------------------------------------------------------------

/**
 * @brief Convert the world coordinates into the chunk coordinates
 *
 * @param c_in the world coordinates
 * @return the equivalent chunk coordinate
 */
CVT_COORDINATE_HEADER(WorldCoordinate, ChunkCoordinate) {
  return ChunkCoordinate(std::floor(c_in.x / CHUNK_SIZE),
                         std::floor(c_in.z / CHUNK_SIZE));
}
/**
 * @brief Convert the chunk coordinates into the region coordinates
 *
 * @param c_in the chunk coordinates
 * @return the equivalent region coordinate
 */
CVT_COORDINATE_HEADER(ChunkCoordinate, RegionCoordinate) {
  return RegionCoordinate(std::floor((float)(c_in.x) / REGION_WIDTH_CHUNK),
                          std::floor((float)(c_in.z) / REGION_WIDTH_CHUNK));
}
/**
 * @brief Convert the world coordinates into the region coordinates
 *
 * @param c_in the world coordinates
 * @return the equivalent region coordinate
 */
CVT_COORDINATE_HEADER(WorldCoordinate, RegionCoordinate) {
  return RegionCoordinate(std::floor(c_in.x / REGION_WIDTH_BLOCK),
                          std::floor(c_in.z / REGION_WIDTH_BLOCK));
}

} // namespace solis::world

#endif