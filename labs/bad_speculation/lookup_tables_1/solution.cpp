#include "solution.hpp"

#ifdef SOLUTION
const static int buckets[101] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // thirteen 0s
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // sixteen 1s
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, // twelve 2s
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, // twelve 3s
    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, // eighteen 4s
    5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, // twelve 5s
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, // seventeen 6s
    DEFAULT_BUCKET
};
static std::size_t mapToBucket(std::size_t v) {
  constexpr auto Nelements = sizeof (buckets) / sizeof (int);
  return buckets[std::min(v, Nelements - 1)];
}

#else

static std::size_t mapToBucket(std::size_t v) {
                              //   size of a bucket
  if      (v < 13)  return 0; //   13
  else if (v < 29)  return 1; //   16
  else if (v < 41)  return 2; //   12
  else if (v < 53)  return 3; //   12
  else if (v < 71)  return 4; //   18
  else if (v < 83)  return 5; //   12
  else if (v < 100) return 6; //   17
  return DEFAULT_BUCKET;
}

#endif

std::array<std::size_t, NUM_BUCKETS> histogram(const std::vector<int> &values) {
  std::array<std::size_t, NUM_BUCKETS> retBuckets{0};
  for (auto v : values) {
    retBuckets[mapToBucket(v)]++;
  }
  return retBuckets;
}
