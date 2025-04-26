#ifndef SOLIS_UTILS_TYPES_HPP
#define SOLIS_UTILS_TYPES_HPP

/**
  =================================== SOLIS ===================================

  This file contains type related definitions.

  @author    Meltwin
  @date      05/04/25
  @version   0.0.1
  @copyright Meltwin - 2025
             Distributed under the MIT Licence
  =============================================================================
*/

#include <cstdint>
#include <type_traits>

namespace solis {

// ============================================================================
//    Signed <-> Unsigned conversion
// ============================================================================

#define CHECK_INTEGRAL                                                         \
  static_assert(std::is_integral<T>::value, "T should be an integral type")

template <typename T> struct to_unsigned { CHECK_INTEGRAL; };
template <> struct to_unsigned<int8_t> { typedef uint8_t U; };
template <> struct to_unsigned<int16_t> { typedef uint16_t U; };
template <> struct to_unsigned<int32_t> { typedef uint32_t U; };
template <> struct to_unsigned<int64_t> { typedef uint64_t U; };
template <> struct to_unsigned<uint8_t> { typedef uint8_t U; };
template <> struct to_unsigned<uint16_t> { typedef uint16_t U; };
template <> struct to_unsigned<uint32_t> { typedef uint32_t U; };
template <> struct to_unsigned<uint64_t> { typedef uint64_t U; };

// ============================================================================
//    Endianness conversion
// ============================================================================

#define SOLIS_BIG_ENDIAN _CMAKE_ENDIANNESS

constexpr uint64_t MASK{static_cast<uint8_t>(0x00000000000000ff)};
constexpr uint8_t BSIZE{sizeof(uint8_t) * 8};

/**
 * @brief Transform a big-endian integer (short / int / long) into the
 * equivalent in the current computer endianness (big or little)
 *
 * @tparam T the integral type to convert
 * @tparam NB the size of such type
 * @param v the big-endian value input
 * @return the big or little-endian value depending on the platform
 */
template <typename T, uint8_t NB = sizeof(T)>
constexpr T swap_bytes(const T v) {
  CHECK_INTEGRAL;
  T other = 0;
  for (uint8_t i = 0; i < NB; i++) {
    const T mask = (MASK) << (i * BSIZE);
    const T byte_val = (v & mask) >> (i * BSIZE);
    other = other | (byte_val) << ((NB - i - 1) * BSIZE);
  }
  return other;
}

/**
 * @brief Specialization for the long type
 *
 * @param v the value to swap the bytes of
 * @return the opposite endianess converted value
 */
template <> constexpr uint64_t swap_bytes(const uint64_t v) {
  uint64_t other = 0;
  other = (v & MASK) << (7 * BSIZE);
  other |= (v & (MASK << BSIZE)) << (5 * BSIZE);
  other |= (v & (MASK << 2 * BSIZE)) << (3 * BSIZE);
  other |= (v & (MASK << 3 * BSIZE)) << BSIZE;
  other |= (v & (MASK << 4 * BSIZE)) >> BSIZE;
  other |= (v & (MASK << 5 * BSIZE)) >> (3 * BSIZE);
  other |= (v & (MASK << 6 * BSIZE)) >> (5 * BSIZE);
  other |= (v & (MASK << 7 * BSIZE)) >> (7 * BSIZE);
  return other;
}

/**
 * @brief Transform a big-endian integer (short / int / long) into the
 * equivalent in the current computer endianness (big or little)
 *
 * @tparam T the integral type to convert
 * @tparam NB the size of such type
 * @param v the big-endian value input
 * @return the big or little-endian value depending on the platform
 */
template <typename T>
inline constexpr T FROM_BIG_ENDIAN(const typename to_unsigned<T>::U v) {
  CHECK_INTEGRAL;
#if SOLIS_BIG_ENDIAN
  return v;
#else
  return (T)swap_bytes<typename to_unsigned<T>::U>(v);
#endif
}

/**
 * @brief Transform a little-endian integer (short / int / long) into the
 * equivalent in the current computer endianness (big or little)
 *
 * @tparam T the integral type to convert
 * @tparam NB the size of such type
 * @param v the little-endian value input
 * @return the big or little-endian value depending on the platform
 */
template <typename T>
inline constexpr T FROM_LITTLE_ENDIAN(const typename to_unsigned<T>::U v) {
  CHECK_INTEGRAL;
#if SOLIS_BIG_ENDIAN
  return (T)swap_bytes<typename to_unsigned<T>::U>(v);
#else
  return v;
#endif
}

/**
 * @brief Transform to a big-endian integer (short / int / long) the
 * equivalent in the current computer endianness (big or little)
 *
 * @tparam T the integral type to convert
 * @tparam NB the size of such type
 * @param v the computer-endianess value input
 * @return the big-endian value
 */
template <typename T>
inline constexpr T TO_BIG_ENDIAN(const typename to_unsigned<T>::U v) {
  CHECK_INTEGRAL;
#if SOLIS_BIG_ENDIAN
  return v;
#else
  return (T)swap_bytes<typename to_unsigned<T>::U>(v);
#endif
}

/**
 * @brief Transform to a little-endian integer (short / int / long) the
 * equivalent in the current computer endianness (big or little)
 *
 * @tparam T the integral type to convert
 * @tparam NB the size of such type
 * @param v the computer-endianess value input
 * @return the little-endian value
 */
template <typename T> inline constexpr T TO_LITTLE_ENDIAN(const T v) {
  CHECK_INTEGRAL;
#if SOLIS_BIG_ENDIAN
  return (T)swap_bytes<typename to_unsigned<T>::U>(v);
#else
  return v;
#endif
}

#undef CHECK_INTEGRAL

} // namespace solis

#endif