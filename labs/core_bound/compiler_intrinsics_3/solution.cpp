#include "solution.hpp"
#include <algorithm>

#ifdef SOLUTION

#include <array>
#include <cstdint>
#include <cstring>
#include <type_traits>
#include <vector>

template<class To, class From>
std::enable_if_t<
        sizeof(To) == sizeof(From) &&
                std::is_trivially_copyable_v<From> &&
                std::is_trivially_copyable_v<To>,
        To>
// constexpr support needs compiler magic
bit_cast(const From &src) noexcept {
    To dst;
    std::memcpy(&dst, &src, sizeof(To));
    return dst;
}

template<class T, std::size_t N>
struct SIMDVector {
    struct vector_helper {
        using type __attribute__((__vector_size__(N * sizeof(T)))) = T; // get around GCC restriction
    };
    using builtin_vector = typename vector_helper::type;

    alignas(N * sizeof(T)) std::array<T, N> data{};

    constexpr SIMDVector() {}
    constexpr SIMDVector(T value) {
        for (T &x: this->data) x = value;
    }

    constexpr SIMDVector(std::array<T, N> data) {
        this->data = data;
    }

    constexpr SIMDVector(builtin_vector bv) : SIMDVector{from_builtin_vector(bv)} {
    }

    template<class G>
    constexpr SIMDVector(SIMDVector<G, N> const &other) : SIMDVector{__builtin_convertvector(other.to_builtin_vector(), builtin_vector)} {
    }

    SIMDVector from_builtin_vector(builtin_vector bv) {
        return {bit_cast<std::array<T, N>>(bv)};
    }

    builtin_vector to_builtin_vector() const {
        return bit_cast<builtin_vector>(*this);
    }

    static constexpr SIMDVector load(T const *ptr) {
        SIMDVector res;
        for (std::size_t i = 0; i < N; ++i) res.data[i] = ptr[i];
        return res;
    }

    constexpr void store(T *ptr) {
        for (std::size_t i = 0; i < N; ++i) ptr[i] = this->data[i];
    }

    constexpr SIMDVector operator+(SIMDVector rhs) const {
        return bit_cast<SIMDVector>(to_builtin_vector() + rhs.to_builtin_vector());
    }

    constexpr SIMDVector &operator+=(SIMDVector rhs) {
        return *this = *this + rhs;
    }
};

Position<std::uint32_t> solution(std::vector<Position<std::uint32_t>> const &input) {
#if defined(__AVX512F__)
    constexpr auto native_simd_size = 64;
#elif defined(__AVX__)
    constexpr auto native_simd_size = 32;
#elif defined(__SSE__)
    constexpr auto native_simd_size = 16;
#else
    constexpr auto native_simd_size = 0;
#endif
    static_assert(native_simd_size > 0, "make sure your target CPU supports SIMD!");

    constexpr auto unroll = native_simd_size / sizeof(std::uint64_t);

    std::size_t i = 0;

    using VecU64 = SIMDVector<std::uint64_t, unroll>;
    using VecU32 = SIMDVector<std::uint32_t, unroll>;

    std::array<VecU64, 3> real_accs = {};
    for (; i + unroll <= input.size(); i += unroll) {
        std::array<std::uint32_t, 3 * unroll> casted;
        for (std::size_t j = 0; j < unroll; ++j) {
            casted[3 * j + 0] = input[i + j].x;
            casted[3 * j + 1] = input[i + j].y;
            casted[3 * j + 2] = input[i + j].z;
        }

        for (std::size_t k = 0; k < 3; ++k) {
            real_accs[k] += VecU64{VecU32::load(casted.data() + unroll * k)};
        }
    }

    alignas(128) std::array<std::uint64_t, 3 * unroll> acc;
    for (std::size_t k = 0; k < 3; ++k) {
        real_accs[k].store(acc.data() + unroll * k);
    }

    std::uint64_t x = 0;
    std::uint64_t y = 0;
    std::uint64_t z = 0;

    for (std::size_t j = 0; j < unroll; ++j) {
        x += acc[3 * j + 0];
        y += acc[3 * j + 1];
        z += acc[3 * j + 2];
    }

    for (; i < input.size(); ++i) {
        x += input[i].x;
        y += input[i].y;
        z += input[i].z;
    }

    return {
            static_cast<std::uint32_t>(x / std::max<std::uint64_t>(1, input.size())),
            static_cast<std::uint32_t>(y / std::max<std::uint64_t>(1, input.size())),
            static_cast<std::uint32_t>(z / std::max<std::uint64_t>(1, input.size())),
    };
}

#elif SOLUTION_2
#include <immintrin.h>
Position<std::uint32_t> solution(std::vector<Position<std::uint32_t>> const &input) {
  std::uint64_t x = 0;
  std::uint64_t y = 0;
  std::uint64_t z = 0;

  int i = 0;
  __m256i acc_XYZX = _mm256_setzero_si256();
  __m256i acc_YZXY = _mm256_setzero_si256();
  __m256i acc_ZXYZ = _mm256_setzero_si256();
  // we will process 4 Positions per iteration (4 * 12 bytes or 3 * sizeof(XMM) )
  constexpr int UNROLL = 4;
  auto input_ptr = reinterpret_cast<const __m128i *>(&input[0].x);
  for (; i + UNROLL - 1 < input.size(); i += UNROLL) {
    __m128i XMM_XYZX = _mm_load_si128(input_ptr + 0); // load 128 bits
    __m128i XMM_YZXY = _mm_load_si128(input_ptr + 1);  
    __m128i XMM_ZXYZ = _mm_load_si128(input_ptr + 2);  
    input_ptr += 3;
    __m256i YMM_XYZX = _mm256_cvtepu32_epi64(XMM_XYZX); // 32 bit -> 64 bit (vpmovsxdq)
    __m256i YMM_YZXY = _mm256_cvtepu32_epi64(XMM_YZXY); 
    __m256i YMM_ZXYZ = _mm256_cvtepu32_epi64(XMM_ZXYZ);
    acc_XYZX = _mm256_add_epi64(acc_XYZX, YMM_XYZX); // accumulation
    acc_YZXY = _mm256_add_epi64(acc_YZXY, YMM_YZXY);
    acc_ZXYZ = _mm256_add_epi64(acc_ZXYZ, YMM_ZXYZ);
  }

  // better with vpermq or similar ?

  // reduce acc_XYZX
  x += _mm256_extract_epi64(acc_XYZX, 0);
  y += _mm256_extract_epi64(acc_XYZX, 1);
  z += _mm256_extract_epi64(acc_XYZX, 2);
  x += _mm256_extract_epi64(acc_XYZX, 3);

  // reduce acc_YZXY
  y += _mm256_extract_epi64(acc_YZXY, 0);
  z += _mm256_extract_epi64(acc_YZXY, 1);
  x += _mm256_extract_epi64(acc_YZXY, 2);
  y += _mm256_extract_epi64(acc_YZXY, 3);

  // reduce acc_ZXYZ
  z += _mm256_extract_epi64(acc_ZXYZ, 0);
  x += _mm256_extract_epi64(acc_ZXYZ, 1);
  y += _mm256_extract_epi64(acc_ZXYZ, 2);
  z += _mm256_extract_epi64(acc_ZXYZ, 3);

  // remainder
  for (; i < input.size(); ++i) {
      x += input[i].x;
      y += input[i].y;
      z += input[i].z;
  }

  return {
          static_cast<std::uint32_t>(x / std::max<std::uint64_t>(1, input.size())),
          static_cast<std::uint32_t>(y / std::max<std::uint64_t>(1, input.size())),
          static_cast<std::uint32_t>(z / std::max<std::uint64_t>(1, input.size())),
  };
}

#else

Position<std::uint32_t> solution(std::vector<Position<std::uint32_t>> const &input) {
  std::uint64_t x = 0;
  std::uint64_t y = 0;
  std::uint64_t z = 0;

  for (auto pos: input) {
    x += pos.x;
    y += pos.y;
    z += pos.z;
  }

  return {
          static_cast<std::uint32_t>(x / std::max<std::uint64_t>(1, input.size())),
          static_cast<std::uint32_t>(y / std::max<std::uint64_t>(1, input.size())),
          static_cast<std::uint32_t>(z / std::max<std::uint64_t>(1, input.size())),
  };
}

#endif