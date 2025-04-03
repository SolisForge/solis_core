#ifndef SOLIS_WORLD_DIMENSION_HPP
#define SOLIS_WORLD_DIMENSION_HPP

/**
  =================================== SOLIS ===================================

  This file contains the description of a dimension object to ensure
  format-agnostic world manipulation.

  @author    Geoffrey Côte
  @date      30/03/25
  @version   0.0.1
  @copyright Meltwin - 2025
             Distributed under the MIT Licence
  =============================================================================
*/

#include "solis/world/chunk.hpp"
#include <vector>

namespace solis::world {

/**
 * @brief Structure describing a dimension (e.g. Nether, overworld, end, ...)
 */
struct Dimension {
  /*
   --------------------------------- Typedef ----------------------------------
  */
  typedef std::shared_ptr<Dimension> SharedPtr;
  typedef char DimType_t;
  enum Type : DimType_t { NETHER = -1, OVERWORLD = 0, END = 1 };

  /*
   ------------------------------ Constructor ---------------------------------
  */
public:
  explicit Dimension(DimType_t type, const char *name);

  /*
   --------------------------- Properties methods -----------------------------
  */
public:
  /**
   * @brief Get the name of the dimension.
   */
  inline const char *get_name() const { return name; }

  /**
   * @brief Get the type of the dimension.
   * Note that this is only indicative and should not be be used as an always
   * true value.
   */
  inline DimType_t get_type() const { return world_type; }

  /*
  ------------------------------ Region methods -------------------------------
  */
public:
  /**
   * @brief Get the requested region given its coordinates.
   *
   * @tparam C the coordinate type
   * @param coordinates the coordinates in the region
   * @return a pointer to the region, null if it does not exist
   */
  template <typename C>
  inline Region::SharedPtr
  get_region(const std::shared_ptr<LocalizedStructure<C>> &obj) const {
    return get_region(CoordinateWrapper<C>(obj->coord));
  }

  /**
   * @brief Get the requested region given its coordinates.
   *
   * @tparam C the coordinate type
   * @param coordinates the coordinates in the region
   * @return a pointer to the region, null if it does not exist
   */
  template <typename C>
  inline Region::SharedPtr
  get_region(const CoordinateWrapper<C> &coordinates) const {
    return get_region(
        CoordinateWrapper(cvtCoordinate<RegionCoordinate>(coordinates.coord)));
  }

  /**
   * @brief Get the requested region given its coordinates.
   *
   * @param coordinates the region coordinates
   * @return a pointer to the region, null if it does not exist
   */
  template <>
  Region::SharedPtr
  get_region(const CoordinateWrapper<RegionCoordinate> &coordinate) const;

  // --------------------------------------------------------------------------

  /**
   * @brief Check whether a region with the coordinates of the given object is
   * loaded in memory.
   *
   * @param obj an object whose coordinates can be translated to region
   * coordinates.
   * @return true if the region is loaded, else otherwise
   */
  template <typename C>
  inline Chunk::SharedPtr
  is_region_loaded(const std::shared_ptr<LocalizedStructure<C>> &obj) const {
    return is_region_loaded(CoordinateWrapper(obj->coord));
  }

  /**
   * @brief Check whether a region with the given coordinates is loaded in
   * memory.
   *
   * @param coordinates the coordinates in the region
   * @return true if the region is loaded, else otherwise
   */
  template <typename C>
  inline bool is_region_loaded(const CoordinateWrapper<C> &coordinates) const {
    return is_region_loaded(
        CoordinateWrapper(cvtCoordinate<RegionCoordinate>(coordinates)));
  }

  /**
   * @brief Check whether a region with the given coordinates is loaded in
   * memory.
   *
   * @param coordinates the coordinates of the region
   * @return true if the region is loaded, else otherwise
   */
  template <>
  bool is_region_loaded(
      const CoordinateWrapper<RegionCoordinate> &coordinates) const;

protected:
  /*
   ------------------------------ Chunk methods -------------------------------
  */
public:
  /**
   * @brief Get the requested chunk given an object.
   *
   * @param obj an object whose coordinates can be translated to chunk
   * coordinates.
   * @return a pointer to the chunk, nullptr if it does not exist
   */
  template <typename C>
  inline Chunk::SharedPtr
  get_chunk(const std::shared_ptr<LocalizedStructure<C>> &obj) const {
    return get_chunk(CoordinateWrapper(obj->coord));
  }

  /**
   * @brief Get the requested chunk given its coordinates.
   *
   * @param coordinates a world coordinate inside of the wanted chunk
   * @return a pointer to the chunk, nullptr if it does not exist
   */
  template <typename C>
  inline Chunk::SharedPtr
  get_chunk(const CoordinateWrapper<C> &coordinates) const {
    return get_chunk(
        CoordinateWrapper(cvtCoordinate<ChunkCoordinate>(coordinates)));
  }

  /**
   * @brief Get the requested chunk given its coordinates.
   * @param coordinates the chunk coordinates
   * @return a pointer to the chunk, nullptr if it does not exist
   */
  template <>
  Chunk::SharedPtr
  get_chunk(const CoordinateWrapper<ChunkCoordinate> &coordinates) const;

  // --------------------------------------------------------------------------

  /**
   * @brief Check whether a chunk with the given coordinates is loaded in
   * memory.
   *
   * @param obj an object whose coordinates can be translated to chunk
   * coordinates.
   * @return true if the chunk is loaded, else otherwise
   */
  template <typename C>
  inline bool
  is_chunk_loaded(const std::shared_ptr<LocalizedStructure<C>> &obj) const {
    return is_chunk_loaded(CoordinateWrapper<C>(obj->coord));
  }

  /**
   * @brief Check whether a chunk with the given coordinates is loaded in
   * memory.
   *
   * @param coordinates the coordinates in the chunk
   * @return true if the chunk is loaded, else otherwise
   */
  template <typename C>
  inline bool is_chunk_loaded(const CoordinateWrapper<C> &coordinates) const {
    return is_chunk_loaded(
        CoordinateWrapper(cvtCoordinate<ChunkCoordinate>(coordinates.coord)));
  }

  /**
   * @brief Check whether a chunk with the given coordinates is loaded in
   * memory.
   *
   * @param coordinates the coordinates of the chunk
   * @return true if the chunk is loaded, else otherwise
   */
  template <>
  bool
  is_chunk_loaded(const CoordinateWrapper<ChunkCoordinate> &coordinates) const;

  // --------------------------------------------------------------------------

  /**
   * @brief Add a new chunk in the dimension.
   * @param chunk the chunk to add
   */
  void add_chunk(const Chunk::SharedPtr chunk);

  /*
   -------------------------------- Properties --------------------------------
  */
protected:
  const char *name;                       /// Name of the dimension
  const DimType_t world_type;             // Type of the dimension
  std::vector<Region::SharedPtr> regions; // Loaded regions of the dimension
};

} // namespace solis::world

#endif