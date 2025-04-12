#include "solis/utils/zlib.hpp"
#include "solis/utils/errors.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>

namespace solis {

// ==========================================================================
// File stream methods
// ==========================================================================

//
void ZFStream::open() {
  // Test for file existence
  auto abs_path = std::filesystem::absolute(filename);
  if (input && !std::filesystem::exists(abs_path))
    throw FileNotFoundError(abs_path.c_str());

  // Open file
  handle = fopen(abs_path.c_str(), ((input) ? "rb" : "wb+"));
  if (!handle)
    throw FileIOError();
}

void ZFStream::close() {
  if (handle != nullptr) {
    fclose(handle);
    handle = nullptr;
  }
}

unsigned int ZFStream::readBytes() {
  if (auto read = fread(buffer, sizeof(buffer[0]), CHUNK_SIZE, handle);
      !ferror(handle))
    return read;
  throw FileIOError();
}

void ZFStream::writeBytes(unsigned int N) {
  if (fwrite(buffer, 1, N, handle) != N || ferror(handle))
    throw FileIOError();
}

int ZFStream::eos() {
  if (handle)
    return feof(handle);
  return 1;
}

// ==========================================================================
// String stream methods
// ==========================================================================

//
unsigned int ZSStream::readBytes() {
  auto to_read = std::min(size - index, CHUNK_SIZE);
  string.copy((char *)buffer, to_read, index);
  index += to_read;
  return to_read;
}

void ZSStream::writeBytes([[maybe_unused]] unsigned int N) { ss << buffer; }

int ZSStream::eos() { return index >= size; }

// ==========================================================================
// Internal methods
// ==========================================================================

//
z_stream ZLib::new_stream() {
  z_stream strm;
  strm.zalloc = Z_NULL;
  strm.zfree = Z_NULL;
  strm.opaque = Z_NULL;
  strm.avail_in = 0;
  strm.next_in = Z_NULL;
  return strm;
}

void ZLib::uncompress(const ZStream::SharedPtr &s1,
                      const ZStream::SharedPtr &s2, int8_t format) {
  z_stream strm = new_stream();
  try {
    // Open and initialize streams
    int ret = inflateInit2(&strm, format);
    // int ret = inflateInit(&strm);
    if (ret != Z_OK)
      throw ZLibError(ret, strm.msg);
    s1->open();
    s2->open();

    do {
      // Read data by chunks
      strm.avail_in = s1->readBytes();
      if (strm.avail_in == 0)
        break;
      strm.next_in = s1->buffer;

      // Run inflate on input buffer
      do {
        strm.avail_out = CHUNK_SIZE;
        strm.next_out = s2->buffer;
        ret = inflate(&strm, Z_NO_FLUSH);

        switch (ret) {
        case Z_NEED_DICT:
        case Z_DATA_ERROR:
        case Z_MEM_ERROR:
          throw ZLibError(ret, strm.msg);
        }

        // Write uncompressed bytes to output stream
        unsigned int have = CHUNK_SIZE - strm.avail_out;
        s2->writeBytes(have);
      } while (strm.avail_out == 0);
    } while (ret != Z_STREAM_END);

    // Close handles
    (void)inflateEnd(&strm);
    s1->close();
    s2->close();
  } catch (const FileIOError e) {
    (void)inflateEnd(&strm);
    s1->close();
    s2->close();
    throw e;
  } catch (const ZLibError e) {
    (void)inflateEnd(&strm);
    s1->close();
    s2->close();
    throw e;
  }
}

void ZLib::compress(const ZStream::SharedPtr &s1, const ZStream::SharedPtr &s2,
                    const unsigned char level, int8_t format) {
  z_stream strm = new_stream();
  try {
    // Open and initialize streams
    int ret = deflateInit2(&strm, level, Z_DEFLATED, format, MAX_MEM_LEVEL,
                           Z_DEFAULT_STRATEGY);
    if (ret != Z_OK)
      throw ZLibError(ret, strm.msg);
    s1->open();
    s2->open();

    int flush;
    do {
      // Read data by chunks
      strm.avail_in = s1->readBytes();
      if (strm.avail_in == 0)
        break;
      strm.next_in = s1->buffer;

      flush = s1->eos() ? Z_FINISH : Z_NO_FLUSH;

      // Run inflate on input buffer
      do {
        strm.avail_out = CHUNK_SIZE;
        strm.next_out = s2->buffer;
        ret = deflate(&strm, flush);
        switch (ret) {
        case Z_NEED_DICT:
        case Z_DATA_ERROR:
        case Z_MEM_ERROR:
          throw ZLibError(ret, strm.msg);
        }

        // Write uncompressed bytes to output stream
        unsigned int have = CHUNK_SIZE - strm.avail_out;
        s2->writeBytes(have);
      } while (strm.avail_out == 0);
    } while (flush != Z_FINISH);

    // Close handles
    (void)deflateEnd(&strm);
    s1->close();
    s2->close();
  } catch (const FileIOError e) {
    (void)deflateEnd(&strm);
    s1->close();
    s2->close();
  } catch (const ZLibError e) {
    (void)deflateEnd(&strm);
    s1->close();
    s2->close();
  }
}

} // namespace solis