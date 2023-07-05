
#include "solution.h"

// Select items which have S.first in range [lower..upper]
std::size_t select(std::array<S, N> &output, const std::array<S, N> &input,
                   const std::uint32_t lower, const std::uint32_t upper) {
  std::size_t count = 0;
  for (const auto item : input) {
#ifdef SOLUTION
    bool in_range = (lower <= item.first) && (item.first <= upper);
    output[count] = item;
    count += in_range;  // Increment count only if in_range is true.
#else
    if ((lower <= item.first) && (item.first <= upper)) {
      output[count++] = item;
    }
#endif
  }
  return count;
}
