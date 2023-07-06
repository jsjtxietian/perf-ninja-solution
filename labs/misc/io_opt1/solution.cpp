#include "solution.hpp"

#include <fstream>
#include <stdexcept>

#include "MappedFile.hpp"

uint32_t solution(const char *file_name) {
#ifdef SOLUTION

  // Initial value has all bits set to 1
  uint32_t crc = 0xff'ff'ff'ff;

  MappedFile mappedFile = MappedFile{file_name};
  std::string_view content = mappedFile.getContents();
  for (const auto& c : content) {
    update_crc32(crc, static_cast<uint8_t>(c));
  }
#else
  std::fstream file_stream{file_name};
  if (not file_stream.is_open())
    throw std::runtime_error{"The file could not be opened"};

  // Initial value has all bits set to 1
  uint32_t crc = 0xff'ff'ff'ff;

  // Update the CRC32 value character by character
  char c;
  while (true) {
    file_stream.read(&c, 1);
    if (file_stream.eof())
      break;
    update_crc32(crc, static_cast<uint8_t>(c));
  }
#endif
  // Invert the bits
  crc ^= 0xff'ff'ff'ff;

  return crc;
}
