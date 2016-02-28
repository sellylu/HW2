# Special Project HW2

## Introduction

All the test results are stored in [sellylu/HW2/Result/](https://github.com/sellylu/HW2/tree/master/Result).

Since I'm using MacBook, I import libraries of opencv with directly creating CMakeLists.txt. If it's possible for Windows or Linux Ubuntu users to use `cmake`, following steps below might be possible re-compile and run exacutable files in other operating systems.

```shell
$ cd <dir>
$ cmake .
$ make
$ ./<executable_file>
```

## Specification

1. **Linear Filter:** processing on the pictures with specified filter in linear way.
2. **Non-linear Filoter:** processing on the pictures of any size filter in non-linear way, such as median or max/min filter.

## Concept Development

## Implementation

### Q1
* Linear filter
* Any size with specified filter mask
<br/>
Flow : 
  1. Padding
  2. 2D convolution ij is the index within the filter window
    y = sum{ Pij*Wij }

### Q2
* Nonlinear filter
* Any size, max/min median
<br/>
Flow :
  1. Padding
  2. Sort Pij within the filter window
  3. Choose max/min/median as result

### Q3
* Sobel
* Gaussian
* Laplacian
* Unsharp (USM)
** Amount (A)
** Radious (R)
** Threshold (T)

Based on Q1, test and compare the effect of different filter

## Discussion

## Reference

- R. C. Gonzalez, R. E. Woods, *"Digital Image Processing"*