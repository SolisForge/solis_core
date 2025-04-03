#ifndef SOLIS_UTILS_FLAGS_HPP
#define SOLIS_UTILS_FLAGS_HPP

/**
  =================================== SOLIS ===================================

  This file contains flags definition and manipulation methods.

  @author    Geoffrey Côte
  @date      30/03/25
  @version   0.0.1
  @copyright Meltwin - 2025
             Distributed under the MIT Licence
  =============================================================================
*/

namespace solis {

typedef unsigned char FlagC_t;
typedef unsigned short FlagS_t;
template <typename T> struct TFlag { T flag; };

typedef TFlag<FlagC_t> Flag;
typedef TFlag<FlagS_t> ShortFlag;

#define FLAG(name, value) static constexpr Flag name{value};

template <typename T>
constexpr bool has_flag(const TFlag<T> v, const TFlag<T> flag) {
  return (v.flag & flag.flag) == flag.flag;
}

template <typename T>
constexpr TFlag<T> operator+(const TFlag<T> &f1, const TFlag<T> &f2) {
  return solis::Flag{static_cast<T>(f1.flag + f2.flag)};
}

} // namespace solis

#endif