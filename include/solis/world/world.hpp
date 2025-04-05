#ifndef SOLIS_WORLD_HPP
#define SOLIS_WORLD_HPP

/**
  =================================== SOLIS ===================================

  This file contains the description of a world object to ensure
  format-agnostic world manipulation.

  @author    Geoffrey Côte
  @date      30/03/25
  @version   0.0.1
  @copyright Meltwin - 2025
             Distributed under the MIT Licence
  =============================================================================
*/

#include "solis/world/coordinates.hpp"
#include "solis/world/dimension.hpp"
#include "solis/world/typedef.hpp"
#include <unordered_map>
#include <vector>

namespace solis::world {

/**
 * @brief Structure describing a world that contains several dimensions.
 */
struct World {
  /*
   --------------------------------- Typedef ----------------------------------
  */
  typedef std::shared_ptr<World> SharedPtr;

  /*
   ------------------------------- Constructors -------------------------------
  */
  explicit World() {}

  /*
   -------------------------------- Methods -----------------------------------
  */
public:
  /**
   * @brief Get the dimension corresponding to the given name
   *
   * @param name the name of the dimension
   * @return a pointer to the dimension, nullptr if it does not exist
   */
  Dimension::SharedPtr get_dimension(const char *name) const;

  /**
   * @brief Get the chunk at the given coordinates in the given dimension.
   *
   * @param dimension the dimension to look into
   * @param coord the chunk coordinates
   * @return a pointer to the chunk object
   */
  Chunk::SharedPtr get_chunk(const char *dim,
                             const ChunkCoordinate &coord) const {
    auto _d = get_dimension(dim);
    if (_d == nullptr)
      return nullptr;
    return _d->get_chunk(coord);
  }

  /**
   * @brief Get the chunk at the given coordinates in the given dimension.
   *
   * @param dim the dimension to look into
   * @param coord the world coordinates in the given chunk
   * @return a pointer to the chunk object
   */
  inline Chunk::SharedPtr get_chunk(const char *dim,
                                    const WorldCoordinate &coord) const {
    return get_chunk(dim, cvtCoordinate<ChunkCoordinate>(coord));
  }

  /*
   -------------------------------- Properties --------------------------------
  */
protected:
  std::unordered_map<const char *, Dimension::SharedPtr> dimensions;
};

} // namespace solis::world

#endif