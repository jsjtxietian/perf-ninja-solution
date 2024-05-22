#include "solution.hpp"
#include <algorithm>
#include <random>

// Init random starting grid of the game
Life::Grid initRandom() {
  std::random_device r;
  std::mt19937_64 random_engine(r());
  std::uniform_int_distribution<int> distrib(0, 9);

  int paddedXDimension = GridXDimension + 2;
  int paddedYDimension = GridYDimension + 2;

  Life::Grid retGrid(paddedXDimension, std::vector<int>(paddedYDimension, 0));

  for (int i = 1; i < paddedXDimension - 1; i++) {
    for (int j = 1; j < paddedYDimension - 1; j++) {
      // 70% dead cells and 30% alive cells
      retGrid[i][j] = distrib(random_engine) > 6;
    }
  }

  return retGrid;
}