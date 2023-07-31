todo: https://github.com/dendibakh/perf-challenge6/commit/62ebb0a4667da938e94995bc5531f94dd118e00d

检查large的正确性；ifstream全读进来一次处理；async io；direct io

## Check Result

cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_CXX_COMPILER="clang++" -DCMAKE_C_COMPILER="clang" -G Ninja ..
cmake --build . --config RelWithDebInfo --parallel 8
cmake --build . --target validate
cmake --build . --target benchmark

vtune can run simple_wordcount.exe

python check_speedup.py -challenge_path ./ -bench_lib_path ../tools/benchmark -num_runs 3

## Result
### After Mmap

| Benchmark | Time | CPU | Time Old | Time New | CPU Old | CPU New |
| ---- | ---- | ---- | ---- | ---- | ---- | ---- |
| OVERALL_GEOMEAN | -0.8233 | -0.8156 | 143 | 25 | 73 | 13 |

### After pmr
| Benchmark | Time | CPU | Time Old | Time New | CPU Old | CPU New |
| ---- | ---- | ---- | ---- | ---- | ---- | ---- |
| OVERALL_GEOMEAN |  -0.1135 | -0.1831 | 25 | 22 | 16 | 13 |

## Solution：

[Performance analysis and tuning challenge #6. | Easyperf](https://easyperf.net/blog/2022/05/28/Performance-analysis-and-tuning-contest-6)

[Performance Tuning Challenge #6 (wordcount) - YouTube](https://www.youtube.com/watch?v=R_yX0XjdSBY)