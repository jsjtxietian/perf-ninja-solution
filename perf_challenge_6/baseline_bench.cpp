
#include "wordcount.hpp"
#include "HybridDetect.h"
#include <iostream>


static std::string inputsDirPath;

int main(int argc, char **argv) {
  GetInfoAndSetPCore();
  constexpr int mandatoryArgumentsCount = 1;
  if (argc < 1 + mandatoryArgumentsCount) {
    std::cerr << "Usage: lab path/to/inputs [gbench args]" << std::endl;
    return 1;
  }
  inputsDirPath = argv[1];
  wordcount(inputsDirPath);
  return 0;
}
