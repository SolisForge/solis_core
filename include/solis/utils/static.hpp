#ifndef SOLIS_UTILS_STATIC_HPP
#define SOLIS_UTILS_STATIC_HPP

/**
  =================================== SOLIS ===================================

  This file contains definitions for compile-time utils.

  @author    Meltwin
  @date      05/04/25
  @version   0.0.1
  @copyright Meltwin - 2025
             Distributed under the MIT Licence
  =============================================================================
*/

#include <type_traits>

namespace solis {

// ============================================================================
//    Specialization testing
// ============================================================================

template <class T, template <class...> class Template>
struct is_specialization : std::false_type {};
template <template <class...> class Template, class... Args>
struct is_specialization<Template<Args...>, Template> : std::true_type {};

} // namespace solis

#endif