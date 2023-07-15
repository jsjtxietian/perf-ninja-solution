#include "solution.hpp"
#include <algorithm>
#include <iostream>


bool solution(MatrixOfDoubles &in, MatrixOfDoubles &out) {
  int size = in.size();
#ifdef SOLUTION
  static constexpr int TILE_SIZE = 16;
  for (int ii = 0; ii < size; ii+= TILE_SIZE) {
    for (int jj = 0; jj < size; jj+= TILE_SIZE) {
      for (int i = ii; i < std::min(ii + TILE_SIZE, size); i++) {
        for (int j = jj; j < std::min(jj + TILE_SIZE, size); j++) {
          out[i][j] = in[j][i];
        }
      }
    }
  }
#else
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      out[i][j] = in[j][i];
    }
  }
#endif
  return out[0][size - 1];
}
