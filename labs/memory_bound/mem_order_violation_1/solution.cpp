
#include "solution.h"
#include <algorithm>
#include <fstream>
#include <stdint.h>
#include <cmath>
#include <ios>

// ******************************************
// ONLY THE FOLLOWING FUNCTION IS BENCHMARKED
// Compute the histogram of image pixels
std::array<uint32_t, 256> computeHistogram(const GrayscaleImage& image) {
#if SOLUTION
  // vbins[bin * simd_width + lane] = count
  // inspired by https://gist.github.com/Nugine/31daa17af55c1973009e2783e73fd70e  
  // slightly worse than the golden solution below
  
  // std::array<uint32_t, 256> hist;
  // hist.fill(0);

  // const int total_pixels = image.width * image.height;
  // const uint8_t* data = image.data.get();

  // // Initialize a large temporary histogram to reduce write conflicts
  // alignas(64) uint32_t vbins[256 * 16];
  // memset(vbins, 0, sizeof(vbins));

  // const int simd_width = 16; // Processing 16 pixels at a time

  // // Process pixels in chunks of 16
  // int i = 0;
  // for (; i <= total_pixels - simd_width; i += simd_width) {
  //     // Load 16 pixels into a 128-bit SIMD register
  //     __m128i x0 = _mm_loadu_si128((__m128i*)(data + i)); // Load 16 bytes

  //     // Convert uint8_t to uint32_t (expanding to 16 elements)
  //     __m512i x2 = _mm512_cvtepu8_epi32(x0);

  //     // Create indices 0 to 15
  //     const __m512i indices = _mm512_set_epi32(
  //         15, 14, 13, 12, 11, 10, 9, 8,
  //         7, 6, 5, 4, 3, 2, 1, 0);

  //     // Compute x3 = (x2 << 4) + indices
  //     // This ensures unique indices for each pixel to avoid write conflicts
  //     __m512i x3 = _mm512_add_epi32(_mm512_slli_epi32(x2, 4), indices);

  //     // Gather counts from vbins
  //     __m512i y0 = _mm512_i32gather_epi32(x3, vbins, 4);

  //     // Increment counts
  //     __m512i y1 = _mm512_add_epi32(y0, _mm512_set1_epi32(1));

  //     // Scatter updated counts back to vbins
  //     _mm512_i32scatter_epi32(vbins, x3, y1, 4);
  // }

  // // Handle any remaining pixels
  // for (; i < total_pixels; ++i) {
  //     uint8_t pixel_value = data[i];
  //     hist[pixel_value]++;
  // }

  // // Accumulate the counts from vbins into the final histogram
  // for (int bin = 0; bin < 256; ++bin) {
  //     // Sum the 16 counts for each bin
  //     __m512i bin_counts = _mm512_loadu_si512(vbins + bin * 16);
  //     uint32_t sum = _mm512_reduce_add_epi32(bin_counts);
  //     hist[bin] += sum;
  // }

  // return hist;
  std::array<uint32_t, 256> hist1;
  std::array<uint32_t, 256> hist2;
  std::array<uint32_t, 256> hist3;
  std::array<uint32_t, 256> hist4;
  hist1.fill(0);
  hist2.fill(0);
  hist3.fill(0);
  hist4.fill(0);
  int i = 0;
  for (; i + 3 < image.width * image.height; i += 4) {
    hist1[image.data[i+0]]++;
    hist2[image.data[i+1]]++;
    hist3[image.data[i+2]]++;
    hist4[image.data[i+3]]++;
  }
  for (; i < image.width * image.height; ++i)
    hist1[image.data[i]]++;

  for (int i = 0; i < hist1.size(); ++i)
    hist1[i] += hist2[i] + hist3[i] + hist4[i];

  return hist1;
#else
  std::array<uint32_t, 256> hist;
  hist.fill(0);
  for (int i = 0; i < image.width * image.height; ++i)
    hist[image.data[i]]++;
  return hist;
#endif
}
// ******************************************

// Calculate Otsu's Threshold
int calcOtsuThreshold(const std::array<uint32_t, 256>& hist, int totalPixels) {
  // normalize histogram
  std::array<double, 256> normHist;
  for (int i = 0; i < 256; ++i)
    normHist[i] = (double)hist[i] / totalPixels;

  double maxVariance = 0;
  int optimalThreshold = 0;

  // Find the optimal threshold
  for (int t = 0; t < 256; ++t) {
    double weight1 = 0, weight2 = 0, mean1 = 0, mean2 = 0;

    for (int i = 0; i <= t; ++i) {
      weight1 += normHist[i];
      mean1 += i * normHist[i];
    }

    for (int i = t + 1; i < 256; ++i) {
      weight2 += normHist[i];
      mean2 += i * normHist[i];
    }

    if (weight1 == 0 || weight2 == 0) continue;

    mean1 /= weight1;
    mean2 /= weight2;

    double variance = weight1 * weight2 * std::pow(mean1 - mean2, 2);

    if (variance > maxVariance) {
      maxVariance = variance;
      optimalThreshold = t;
    }
  }

  return optimalThreshold;
}

// Function to apply the threshold to create a binary image
void applyOtsuThreshold(GrayscaleImage& image) {
  // Compute the histogram
  std::array<uint32_t, 256> hist = computeHistogram(image);
  auto totalPixels = image.height * image.width;
  int threshold = calcOtsuThreshold(hist, totalPixels);
  // Apply Otsu's thresholding
  for (int i = 0; i < totalPixels; ++i)
    image.data[i] = (image.data[i] >= threshold) ? 255 : 0;
}

// Loads GrayscaleImage image. Format is
// https://people.sc.fsu.edu/~jburkardt/data/pgmb/pgmb.html 
bool GrayscaleImage::load(const std::string &filename, const int maxSize) {
  data.reset();

  std::ifstream input(filename.data(),
                      std::ios_base::in | std::ios_base::binary);
  if (input.is_open()) {
    std::string line;
    input >> line;
    if (line == "P5") {
      int amplitude;
      input >> width >> height >> amplitude;

      char c;
      input.unsetf(std::ios_base::skipws);
      input >> c;

      // Skip both '\r' and '\n' characters
      if (c == '\r') {
          input >> c;
      }

      if ((width > 0) && (width <= maxSize) && (height > 0) &&
          (height <= maxSize) && (amplitude >= 0) && (amplitude <= 255) &&
          (c == '\n')) {
        size = static_cast<size_t>(width) * static_cast<size_t>(height);
        data.reset(new uint8_t[size]);
        if (data) {
          input.read(reinterpret_cast<char *>(data.get()), size);
          if (input.fail()) {
            data.reset();
          }
        }
      }
    }

    input.close();
  }

  return !!data;
}

// Saves GrayscaleImage image. Format is
// https://people.sc.fsu.edu/~jburkardt/data/pgmb/pgmb.html
void GrayscaleImage::save(const std::string &filename) {
  std::ofstream output(filename.data(),
                       std::ios_base::out | std::ios_base::binary);
  if (output.is_open()) {
    output << "P5" << std::endl
           << width << ' ' << height << std::endl
           << "255" << std::endl;
    if (data) {
      output.write(reinterpret_cast<const char *>(data.get()), size);
    }
    output.close();
  }
}
