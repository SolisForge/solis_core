#ifndef SOLIS_UTILS_FLAGS_HPP
#define SOLIS_UTILS_FLAGS_HPP

/**
  =================================== SOLIS ===================================

  This file contains flags definition and manipulation methods.

  @author    Meltwin
  @date      30/03/25
  @version   0.0.1
  @copyright Meltwin - 2025
             Distributed under the MIT Licence
  =============================================================================
*/

#include <cstdint>
#include <type_traits>
namespace solis {

// ============================================================================
// Flag types
// ============================================================================

//
typedef uint8_t FlagC_t;
typedef uint16_t FlagS_t;
typedef uint32_t FlagI_t;
typedef uint64_t FlagL_t;

// ============================================================================
// Flag types wrapper
// ============================================================================

template <typename T> struct TFlag { T flag; };
typedef TFlag<FlagC_t> Flag;
typedef TFlag<FlagS_t> ShortFlag;
typedef TFlag<FlagI_t> IntFlag;
typedef TFlag<FlagL_t> LongFlag;

#define CFLAG(name, value) static constexpr solis::FlagC_t name = value;
#define FLAG(name, value) static constexpr solis::Flag name = value;

// ============================================================================
// Flag operations
// ============================================================================

template <typename T> constexpr bool has_flag(const T v, const T flag) {
  static_assert(std::is_integral<T>::value, "Flag type should be integral !");
  return (v & flag) == flag;
}

template <typename T>
constexpr bool has_flag(const TFlag<T> v, const TFlag<T> flag) {
  static_assert(std::is_integral<T>::value, "Flag type should be integral !");
  return (v.flag & flag.flag) == flag.flag;
}

template <typename T>
constexpr TFlag<T> operator|(const TFlag<T> &f1, const TFlag<T> &f2) {
  static_assert(std::is_integral<T>::value, "Flag type should be integral !");
  return solis::Flag{static_cast<T>(f1.flag | f2.flag)};
}

} // namespace solis

#endif