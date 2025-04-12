#ifndef SOLIS_UTILS_ZLIB_HPP
#define SOLIS_UTILS_ZLIB_HPP

/**
  =================================== SOLIS ===================================

  This file contains the implementation of a ZLIB wrapper.

  @author    Meltwin
  @date      30/03/25
  @version   0.0.1
  @copyright Meltwin - 2025
             Distributed under the MIT Licence
  =============================================================================
*/

#include <memory>
#include <sstream>
#include <string>

#if defined(_WIN32) && !defined(_CRT_NONSTDC_NO_DEPRECATE)
#define _CRT_NONSTDC_NO_DEPRECATE
#endif
#include <zlib.h>
#if defined(MSDOS) || defined(OS2) || defined(WIN32) || defined(__CYGWIN__)
#include <fcntl.h>
#include <io.h>
#define SET_BINARY_MODE(file) setmode(fileno(file), O_BINARY)
#else
#define SET_BINARY_MODE(file)
#endif

// Define windows bits for several algorithms

namespace solis {

constexpr size_t CHUNK_SIZE{16384};

// ----------------------------------------------------------------------------

/**
 * @brief Stream interface to allow running zlib operations on both files and
 * string.
 */
struct ZStream {
  typedef std::shared_ptr<ZStream> SharedPtr;
  unsigned char buffer[CHUNK_SIZE];

  virtual void open() = 0;
  virtual unsigned int readBytes() = 0;
  virtual void writeBytes(unsigned int) = 0;
  virtual void close() = 0;

  /**
   * @brief Whether we reached end of stream.
   * @return nonzero if end of stream, 0 otherwise
   */
  virtual int eos() = 0;
};

// ----------------------------------------------------------------------------

/**
 * @brief
 *
 */
struct ZFStream final : ZStream {
  typedef std::shared_ptr<ZFStream> SharedPtr;
  ZFStream(const char *fname, bool _input = true)
      : input(_input), filename(fname) {}

  static ZFStream::SharedPtr make(const char *fname, bool _input = true) {
    return std::make_shared<ZFStream>(fname, _input);
  }

  ~ZFStream() { close(); }

  void open() override;
  void close() override;
  unsigned int readBytes() override;
  void writeBytes(unsigned int) override;
  int eos() override;

protected:
  FILE *handle = nullptr;
  bool input;
  const char *filename;
};

// ----------------------------------------------------------------------------

/**
 * @brief Stream implementation for strings.
 */
struct ZSStream final : ZStream {
  typedef std::shared_ptr<ZSStream> SharedPtr;
  explicit ZSStream() {}
  explicit ZSStream(const std::string &input)
      : string(input), size(input.size()) {}

  static ZSStream::SharedPtr make() { return std::make_shared<ZSStream>(); }
  static ZSStream::SharedPtr make(const std::string &input) {
    return std::make_shared<ZSStream>(input);
  }

  void open() override {}
  void close() override {}
  unsigned int readBytes() override;
  void writeBytes(unsigned int) override;
  int eos() override;

  std::string str() { return ss.str(); }

protected:
  std::stringstream ss;
  std::string string;
  size_t index = 0, size;
};

// ----------------------------------------------------------------------------

/**
 * @brief ZLIB wrapper for easier compressed file manipulation.
 */
struct ZLib {
  static constexpr int8_t FORMAT_ZLIB{MAX_WBITS};
  static constexpr int8_t FORMAT_DEFLATE{-MAX_WBITS};
  static constexpr int8_t FORMAT_GZIP{MAX_WBITS | 16};

  // ==========================================================================
  // Decode instructions
  // ==========================================================================
public:
  /**
   * @brief Decode a file content and export it to a string object.
   *
   * @param file_in the file path to read (compressed data)
   * @return a string object containing the decoded bytes
   */
  static std::string decodeFromFile(const char *file_in,
                                    int8_t format = FORMAT_GZIP) {
    auto input = ZFStream::make(file_in);
    auto output = ZSStream::make();
    uncompress(input, output, format);
    return output->str();
  }
  /**
   * @brief Decode a string content and export it to a string object.
   *
   * @param in the compressed string
   * @return a string object containing the decoded bytes
   */
  static std::string decodeFromString(const std::string &in,
                                      int8_t format = FORMAT_GZIP) {
    auto input = ZSStream::make(in);
    auto output = ZSStream::make();
    uncompress(input, output, format);
    return output->str();
  }
  /**
   * @brief Decode a file content and export it to another file.
   *
   * @param file_in the input file (compressed)
   * @param file_out the output file (uncompressed)
   */
  static void decodeAndSaveFromFile(const char *file_in, const char *file_out,
                                    int8_t format = FORMAT_GZIP) {
    auto input = ZFStream::make(file_in);
    auto output = ZFStream::make(file_out, false);
    uncompress(input, output, format);
  }
  /**
   * @brief Decode a string content and export it to another file.
   *
   * @param in the compressed string
   * @param file_out the output file (uncompressed)
   */
  static void decodeAndSaveFromString(const std::string &in,
                                      const char *file_out,
                                      int8_t format = FORMAT_GZIP) {
    auto input = ZSStream::make(in);
    auto output = ZFStream::make(file_out, false);
    uncompress(input, output, format);
  }

  // ==========================================================================
  // Encode instructions
  // ==========================================================================
public:
  /**
   * @brief Encode a file content and export it to a string object.
   *
   * @param file_in the file path to read (uncompressed data)
   * @return a string object containing the encoded bytes
   */
  static std::string encodeFromFile(const char *file_in,
                                    unsigned char level = 6,
                                    int8_t format = FORMAT_GZIP) {
    auto input = ZFStream::make(file_in);
    auto output = ZSStream::make();
    compress(input, output, level, format);
    return output->str();
  }
  /**
   * @brief Encode a string content and export it to a string object.
   *
   * @param in the uncompressed string
   * @return a string object containing the encoded bytes
   */
  static std::string encodeFromString(const std::string &in,
                                      unsigned char level = 6,
                                      int8_t format = FORMAT_GZIP) {
    auto input = ZSStream::make(in);
    auto output = ZSStream::make();
    compress(input, output, level, format);
    return output->str();
  }
  /**
   * @brief Encode a file content and export it to another file.
   *
   * @param file_in the input file (uncompressed)
   * @param file_out the output file (compressed)
   */
  static void encodeAndSaveFromFile(const char *file_in, const char *file_out,
                                    unsigned char level = 6,
                                    int8_t format = FORMAT_GZIP) {
    auto input = ZFStream::make(file_in);
    auto output = ZFStream::make(file_out, false);
    compress(input, output, level, format);
  }
  /**
   * @brief Encode a string content and export it to another file.
   *
   * @param in the uncompressed string
   * @param file_out the output file (compressed)
   */
  static void encodeAndSaveFromString(const std::string &in,
                                      const char *file_out,
                                      unsigned char level = 6,
                                      int8_t format = FORMAT_GZIP) {
    auto input = ZSStream::make(in);
    auto output = ZFStream::make(file_out, false);
    compress(input, output, level, format);
  }

  // ==========================================================================
  // Internal methods
  // ==========================================================================
protected:
  /**
   * @brief Create a new configured z_stream object
   */
  inline static z_stream new_stream();

  static void uncompress(const ZStream::SharedPtr &s1,
                         const ZStream::SharedPtr &s2,
                         int8_t format = FORMAT_GZIP);

  static void compress(const ZStream::SharedPtr &s1,
                       const ZStream::SharedPtr &s2, unsigned char level = 6,
                       int8_t format = FORMAT_GZIP);
};

} // namespace solis

#endif