How to build:

```shell
g++ -O3 -g test.cpp kernel.cpp `pkg-config --libs OpenCL` -o opencl-mandelbrot
```

This requires e.g. the nvidia-opencl-dev to be installed (or something similar for AMD).

After that, invoke:

```shell
./opencl-mandelbrot
```

After a while (initialization takes by far the longest) a `test.pgm` file is created. Most Linux picture viewers can open that.


Folkert van Heusden
