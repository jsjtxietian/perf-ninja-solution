# Performance Analysis and Tuning Challenge #4

Canny image edge detector algorithm

args: 221575.pgm 0.5 0.7 0.9

Read the details here:
https://easyperf.net/blog/2021/02/05/Performance-analysis-and-tuning-contest-4

https://www.youtube.com/watch?v=xvnFfl9LctM

https://easyperf.net/blog/2021/03/27/Summary-of-contest-4-part1

Solution:
* base line : ~ 3.1 - 3.3 s
* loop interchange of y blur in `gaussian_smooth`:  ~ 2.5s
* loop interchange in `derrivative_x_y`:  ~ 2.25 - 2.3 s
* simd in `magnitude_x_y`:  ~ 2.1 - 2.15 s
* remove condition in x blur in `gaussian_smooth`:  ~ 2.01 - 2.02s
* remove the last conditonal store in `non_max_supp`: 1.83 ~ 1.84s (replace 3 nested if with look up won't speed up)
* remove conditonal store in `apply_hysteresis`: 1.67-1.68s
* reduce bad speculation & remove necessary parameters in `follow_edges`: 1.64s (inline follow_edge in itself once won't speed up)
* reduce uncessary calculation in `gaussian_smooth`: 1.60-1.62s
* loop wulti-version in `gaussian_smooth`: 1.58-1.59s

TODO:
* check https://github.com/dendibakh/perf_challenge4/commits/master/Andrey_Evstyukhin
* merge functions, more intrinsics, software prefetch