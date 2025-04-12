#ifndef SOLIS_UTILS_OUTPUT_HPP
#define SOLIS_UTILS_OUTPUT_HPP

/**
  =================================== SOLIS ===================================

  This file contains some functions to outputs data.

  @author    Meltwin
  @date      30/03/25
  @version   0.0.1
  @copyright Meltwin - 2025
             Distributed under the MIT Licence
  =============================================================================
*/

#include <iostream>

namespace solis {

/**
 * @brief Dump the given buffer as hex code in the stdout
 *
 * @param buffer the buffer to dump
 * @param N the length of the buffer
 */
inline void dump_hex(const unsigned char *buffer, unsigned long N) {
  for (unsigned long i = 0; i < N; ++i) {
    fprintf(stdout, "%02x", buffer[i]);
    if ((i + 1) % 8 == 0) {
      fputc(' ', stdout);
    }
    if ((i + 1) % 32 == 0) {
      fputc('\n', stdout);
    }
  }
  fputc('\n', stdout);
}

} // namespace solis

#endif