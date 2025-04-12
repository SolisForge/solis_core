#ifndef SOLIS_UTILS_ERRORS_HPP
#define SOLIS_UTILS_ERRORS_HPP

/**
  =================================== SOLIS ===================================

  This file contains errors definition and manipulation methods.

  @author    Meltwin
  @date      30/03/25
  @version   0.0.1
  @copyright Meltwin - 2025
             Distributed under the MIT Licence
  =============================================================================
*/

#include "solis/utils/common.hpp"
#include <exception>
#include <fmt/format.h>
#include <zlib.h>

namespace solis {

struct FileNotFoundError : public std::exception {
  sconstchar MSG_FORMAT{"Unknown file \"{:s}\"!"};
  FileNotFoundError(const char *filename)
      : error_msg(fmt::format(MSG_FORMAT, filename)) {}
  const char *what() const noexcept { return error_msg.c_str(); }

protected:
  const std::string error_msg;
};

struct ZLibError : public std::exception {
  sconstchar MSG_FORMAT{"ZLib failed with code {:d} ({:s}: {:s})!"};
  ZLibError(int8_t err_code, const char *msg)
      : error_msg(fmt::format(MSG_FORMAT, err_code, code2char(err_code), msg)) {
  }
  const char *what() const noexcept { return error_msg.c_str(); }

  static constexpr const char *code2char(int8_t code) {
    switch (code) {
    case Z_OK:
      return "OK";
    case Z_STREAM_END:
      return "STREAM_END";
    case Z_NEED_DICT:
      return "NEED_DICT";
    case Z_ERRNO:
      return "ERRNO";
    case Z_STREAM_ERROR:
      return "STEAM_ERROR";
    case Z_DATA_ERROR:
      return "DATA_ERROR";
    case Z_MEM_ERROR:
      return "MEM_ERROR";
    case Z_BUF_ERROR:
      return "BUF_ERROR";
    case Z_VERSION_ERROR:
      return "VERSION_ERROR";
    default:
      return "UNKNOWN";
    }
  }

protected:
  const std::string error_msg;
};

struct FileIOError : std::exception {
  sconstchar MSG_FORMAT{"File IO failed with code {:d} ({:s})!"};
  FileIOError() : error_msg(fmt::format(MSG_FORMAT, errno, code2char(errno))) {}

  const char *what() const noexcept { return error_msg.c_str(); }

  static constexpr const char *code2char(int err) {
    switch (err) {
    case EACCES:
      return "EACCES: Permission denied!";
    case EINTR:
      return "EINTR: Interupt caught during open!";
    case EISDIR:
      return "EISDIR: Given path is a directory!";
    case ELOOP:
      return "ELOOP: Too many symbolic link has been followed!";
    case EMFILE:
      return "EMFILE: Too many files are opened in this process!";
    case ENAMETOOLONG:
      return "ENAMETOOLONG: The path name is too long!";
    case ENFILE:
      return "ENFILE: Too many files are opened in the system!";
    case ENOENT:
      return "ENOENT: The given path does not correspond to any existing file!";
    case ENOSPC:
      return "ENOSPC: The directory can't be expanded!";
    case ENOTDIR:
      return "ENOTDIR: A component of the path is not a directory!";
    case ENXIO:
      return "ENXIO: Filename is a special character!";
    case EOVERFLOW:
      return "EOVERFLOW: The size of the file is too big for the type off_t!";
    case EROFS:
      return "EROFS: The file is in a read-only file system and the mode "
             "requires "
             "write-access!";
    case EINVAL:
      return "EINVAL: The mode is invalid!";
    case ENOMEM:
      return "ENOMEM: Not enough storage space available!";
    case ETXTBSY:
      return "ETXTBSY: The file is a pure procedure and mode requires write "
             "access!";
    default:
      return "UNKNOWN";
    }
  }

protected:
  const std::string error_msg;
};

} // namespace solis

#endif