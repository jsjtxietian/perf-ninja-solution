
#include "solution.h"
#include <algorithm>
#include <stdlib.h>

#if SOLUTION
static inline int compare(const void *lhs, const void *rhs) {
#else
static int compare(const void *lhs, const void *rhs) {
#endif
  auto &a = *reinterpret_cast<const S *>(lhs);
  auto &b = *reinterpret_cast<const S *>(rhs);

  if (a.key1 < b.key1)
    return -1;

  if (a.key1 > b.key1)
    return 1;

  if (a.key2 < b.key2)
    return -1;

  if (a.key2 > b.key2)
    return 1;

  return 0;
}

void solution(std::array<S, N> &arr) {
  qsort(arr.data(), arr.size(), sizeof(S), compare);
}
