#include "solution.hpp"
#include <algorithm>
#include <iostream>


bool solution(MatrixOfDoubles &in, MatrixOfDoubles &out) {
  int size = in.size();
#ifdef SOLUTION
  constexpr int cachelineSize = std::hardware_constructive_interference_size / sizeof(double);
  int stride = size / cachelineSize;
  for (int i = 0; i < stride; i++) {
    for (int j = 0; j < stride; j++) {
      for(int m = 0 ; m < cachelineSize ; m++)
        for(int n = 0 ; n < cachelineSize ; n++)
          out[i * cachelineSize + m][j* cachelineSize + n] = in[j* cachelineSize + n][i * cachelineSize + m];
    }
  }

  for (int i = cachelineSize * stride ; i < size ;  i++)
    for (int j = 0; j < size; j++)
      out[i][j] = in[j][i];

  for (int j = cachelineSize * stride ; j < size ;  j++) 
    for (int i = 0; i < size; i++) 
      out[i][j] = in[j][i];

  for (int i = cachelineSize * stride ; i < size ;  i++)
    for (int j = cachelineSize * stride ; j < size ;  j++) 
      out[i][j] = in[j][i];
#else
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      out[i][j] = in[j][i];
    }
  }
#endif
  return out[0][size - 1];
}
