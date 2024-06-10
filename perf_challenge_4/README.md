# Performance Analysis and Tuning Challenge #4

Canny image edge detector algorithm

Read the details here:
https://easyperf.net/blog/2021/02/05/Performance-analysis-and-tuning-contest-4

https://www.youtube.com/watch?v=xvnFfl9LctM

https://easyperf.net/blog/2021/03/27/Summary-of-contest-4-part1

Solution:
* base line : ~ 3.1 - 3.3 s
* loop interchange in `gaussian_smooth`:  ~ 2.5s
* loop interchange in `derrivative_x_y`:  ~ 2.25 - 2.3 s