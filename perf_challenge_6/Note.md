todo: https://github.com/dendibakh/perf-challenge6/commit/62ebb0a4667da938e94995bc5531f94dd118e00d

- [x] 检查large的正确性
- [x] xxhash [Cyan4973/xxHash: Extremely fast non-cryptographic hash algorithm (github.com)](https://github.com/Cyan4973/xxHash) => std::unordered_map + xxhash不显著
- [ ] [How quickly can you remove spaces from a string? – Daniel Lemire's blog](https://lemire.me/blog/2017/01/20/how-quickly-can-you-remove-spaces-from-a-string/)
- [x] software prefetch for hash => 不显著
- [x] lookup table => 不显著
- [x] huge page => 叠加pmr不显著

对比mmap和

- [ ] ifstream全读进来一次处理
- [ ] async io
- [ ] direct io



## Check Result

cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_CXX_COMPILER="clang++" -DCMAKE_C_COMPILER="clang" -G Ninja ..
cmake --build . --config RelWithDebInfo --parallel 8
cmake --build . --target validate
cmake --build . --target benchmark

vtune can run simple_wordcount.exe

python check_speedup.py -challenge_path ./ -bench_lib_path ../tools/benchmark -num_runs 3

42020903 17639696|280387volt.7596[[Joseph1皙殽

## Result
### Mmap vs Origin

| Benchmark | Time | CPU | Time Old | Time New | CPU Old | CPU New |
| ---- | ---- | ---- | ---- | ---- | ---- | ---- |
| OVERALL_GEOMEAN | -0.4173 | -0.3272 | 141 | 82 | 77 | 52 |

### MMap + pmr vs MMap
| Benchmark | Time | CPU | Time Old | Time New | CPU Old | CPU New |
| ---- | ---- | ---- | ---- | ---- | ---- | ---- |
| OVERALL_GEOMEAN |  -0.1375 | -0.0993 | 82 | 71 | 45 | 41 |

### MMap + robin_hood hash vs MMap 
| Benchmark | Time | CPU | Time Old | Time New | CPU Old | CPU New |
| ---- | ---- | ---- | ---- | ---- | ---- | ---- |
| OVERALL_GEOMEAN |  -0.2670 | -0.2809 | 89 | 65 | 52 | 37 |

### MMap + robin_hood hash vs  MMap + pmr
| Benchmark | Time | CPU | Time Old | Time New | CPU Old | CPU New |
| ---- | ---- | ---- | ---- | ---- | ---- | ---- |
| OVERALL_GEOMEAN |  -0.0831 |  -0.1428 | 72 | 66 | 40 | 35 |

https://github.com/martinus/robin-hood-hashing

## Solution：

[Performance analysis and tuning challenge #6. | Easyperf](https://easyperf.net/blog/2022/05/28/Performance-analysis-and-tuning-contest-6)

[Performance Tuning Challenge #6 (wordcount) - YouTube](https://www.youtube.com/watch?v=R_yX0XjdSBY)