#### quick start
* install python packages listed in check_speedup.py
* install gbench use script under tools folder
* install clang and ninja
* code ```#ifdef SOLUTION```
* run ```python .\tools\check_speedup.py -lab_path .\labs\misc\warmup\ -num_runs 3```
```
cmake -E make_directory build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
// cmake -DCMAKE_BUILD_TYPE=Release -G Ninja -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang ..
cmake --build . --config Release --parallel 8 
cmake --build . --target validateLab
cmake --build . --target benchmarkLab
// cmake --build . --config Release --target benchmarkLab
```

#### gprof2dot

[Navigating the Complexity of Large Codebases Using Vtune + xdot (or perf + gprof2dot) - Performance Engineering (pramodkumbhar.com)](https://pramodkumbhar.com/2024/04/navigating-the-complexity-of-large-codebases-using-vtune-xdot-or-perf-gprof2dot/)

vtune.exe --report=gprof-cc --result-dir=r001hs\ --format=text --report-output=sample_profile.txt
python gprof2dot.py  -f axe -n0 -e0 -s sample_profile.txt | dot.exe  -Tsvg -o output.svg