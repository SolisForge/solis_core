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

#include <array>
#include <cstdint>
#include <cstring>
#include <string_view>
#include <type_traits>
#include <variant>

namespace solis {

// ============================================================================
//    Specialization testing
// ============================================================================

template <class T, template <class...> class Template>
struct is_specialization : std::false_type {};
template <template <class...> class Template, class... Args>
struct is_specialization<Template<Args...>, Template> : std::true_type {};

// ============================================================================
//    Compile-time char[] concatenation
// ============================================================================

constexpr std::size_t strlength(const char *m) {
  const char *p = m;
  std::size_t n = 0;
  while (*p != '\0') {
    n++;
    p++;
  }
  return n;
}

template <std::string_view const &...Cs> struct concat {
  static constexpr std::size_t len = (Cs.size() + ... + 0);
  static constexpr auto impl() noexcept {
    std::array<char, len + 1> arr{};
    auto append = [i = 0, &arr](auto const &s) mutable {
      for (auto c : s)
        arr[i++] = c;
    };
    (append(Cs), ...);
    arr[len] = '\0';
    return arr;
  }
  static constexpr auto arr = impl();

  // Views
  static constexpr std::string_view value{arr.data(), arr.size() - 1};
};

template <std::string_view const &...Cs>
static constexpr auto concat_v = concat<Cs...>::value;

// ============================================================================
//    Unsigned <-> Signed conversions
// ============================================================================

#define CHECK_INTEGRAL                                                         \
  static_assert(std::is_integral<T>::value, "T should be an integral type")

template <typename T> struct Unsigned {
  CHECK_INTEGRAL;
  typedef T U;
};
template <typename T> struct Signed {
  CHECK_INTEGRAL;
  typedef T U;
};

#define DEF_SIGNED_CONVERSION(signed_type, unsigned_type)                      \
  template <> struct Unsigned<signed_type> { typedef unsigned_type U; };       \
  template <> struct Signed<unsigned_type> { typedef signed_type U; };
DEF_SIGNED_CONVERSION(int8_t, uint8_t)
DEF_SIGNED_CONVERSION(int16_t, uint16_t)
DEF_SIGNED_CONVERSION(int32_t, uint32_t)
DEF_SIGNED_CONVERSION(int64_t, uint64_t)
#undef DEF_SIGNED_CONVERSION

// ============================================================================
//    Compile-time byte computing
// ============================================================================

template <typename O, typename I, I... A>
inline constexpr auto compute_bytes() {
  static_assert(sizeof(O) == sizeof(I) * sizeof...(A),
                "Not enough data to compute bytes!");
  O out = 0;
  auto add = [i = 1, &out](I to_add) mutable {
    out |= (O)(to_add) << 8 * (sizeof(O) - (i++) * sizeof(I));
  };
  (add(A), ...);
  return out;
}

// ============================================================================
//    Endianness conversion
// ============================================================================

#define SOLIS_BIG_ENDIAN _CMAKE_ENDIANNESS

constexpr uint64_t MASK{static_cast<uint8_t>(0x00000000000000ff)};
constexpr uint8_t BSIZE{sizeof(uint8_t) * 8};

/**
 * @brief Transform a big-endian integer (short / int / long) into the
 * equivalent little equivalent or the inverse operation.
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
template <> constexpr uint16_t swap_bytes(const uint16_t v) {
  uint16_t other = 0;
  other = (v & MASK) << BSIZE;
  other |= (v & (MASK << BSIZE)) >> BSIZE;
  return other;
}

/**
 * @brief Specialization for the long type
 *
 * @param v the value to swap the bytes of
 * @return the opposite endianess converted value
 */
template <> constexpr uint32_t swap_bytes(const uint32_t v) {
  uint32_t other = 0;
  other = (v & MASK) << (3 * BSIZE);
  other |= (v & (MASK << BSIZE)) << (BSIZE);
  other |= (v & (MASK << 2 * BSIZE)) >> BSIZE;
  other |= (v & (MASK << 3 * BSIZE)) >> (3 * BSIZE);
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
template <typename T> inline constexpr T FROM_BIG_ENDIAN(const T v) {
  CHECK_INTEGRAL;
#if SOLIS_BIG_ENDIAN == 1
  return v;
#else
  return (T)swap_bytes<typename Unsigned<T>::U>(v);
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
inline constexpr T FROM_LITTLE_ENDIAN(const typename Unsigned<T>::U v) {
  CHECK_INTEGRAL;
#if SOLIS_BIG_ENDIAN == 1
  return (T)swap_bytes<typename Unsigned<T>::U>(v);
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
inline constexpr T TO_BIG_ENDIAN(const typename Unsigned<T>::U v) {
  CHECK_INTEGRAL;
#if SOLIS_BIG_ENDIAN == 1
  return v;
#else
  return (T)swap_bytes<typename Unsigned<T>::U>(v);
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
#if SOLIS_BIG_ENDIAN == 1
  return (T)swap_bytes<typename Unsigned<T>::U>(v);
#else
  return v;
#endif
}

#undef CHECK_INTEGRAL

} // namespace solis

#endif