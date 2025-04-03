#ifndef SOLIS_BLOCKS_HPP
#define SOLIS_BLOCKS_HPP

/**
  =================================== SOLIS ===================================

  This file contains the description of a block resource.

  @author    Geoffrey Côte
  @date      01/04/25
  @version   0.0.1
  @copyright Meltwin - 2025
             Distributed under the MIT Licence
  =============================================================================
*/

namespace solis {

struct Block {
  const char *package;
  const char *resource_name;

  Block(const char *pkg, const char *name);
};

} // namespace solis

#endif