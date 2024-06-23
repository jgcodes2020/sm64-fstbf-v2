# FST Bruteforcer v2

A rewrite of [SpudY2K's bruteforcer](https://github.com/SpudY2K/sm64-fst-brute-forcer) in SYCL; rewriting the code to take
better advantage of hardware and being more organized in general. Theoretically this should run on any GPU you throw at it;
but I've been fighting with SYCL for at least a little while now.

## Notes

- Currently only testing with Intel oneAPI; which only compiles AMD/NVIDIA on Linux.
