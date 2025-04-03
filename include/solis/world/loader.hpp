#ifndef SOLIS_WORLD_LOADER_HPP
#define SOLIS_WORLD_LOADER_HPP

/**
  =================================== SOLIS ===================================

  This file contains the description of a world loader interface to ensure
  format-agnostic world manipulation.

  @author    Geoffrey Côte
  @date      30/03/25
  @version   0.0.1
  @copyright Meltwin - 2025
             Distributed under the MIT Licence
  =============================================================================
*/

namespace solis {

struct WorldLoader {

  bool load_world(const char *world_name);
};

} // namespace solis

#endif