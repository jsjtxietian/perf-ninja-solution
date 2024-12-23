# Data packing

### NOTE: after migrating from clang12 to clang14 the speedup is no longer measurable.
See https://github.com/dendibakh/perf-ninja/issues/100

This is a lab about data packing.

[<img src="../../../img/DataPacking1Intro.png">](https://www.youtube.com/watch?v=-V-oIXrqA2s&list=PLRWO2AL1QAV6bJAU2kgB4xfodGID43Y5d)

You can decrease the memory traffic of the application if you pack the data more efficiently.
Some of the ways to do that include:

* Eliminate compiler-added padding.
* Use types that require less memory or less precision e.g. (int -> short, double -> float).
* Use bitfields to pack the data even further.

[<img src="../../../img/DataPacking1Summary.png">](https://www.youtube.com/watch?v=ta096PQ6gTg&list=PLRWO2AL1QAV6bJAU2kgB4xfodGID43Y5d)
