How to build:

```shell
mkdir build
cd build
cmake ..
make
```

This requires e.g. the nvidia-opencl-dev to be installed (or something similar for AMD).

After that, invoke:

```shell
./opencl-mandelbrot
```

After a while (initialization takes by far the longest) a `test.pgm` file is created. Most Linux picture viewers can open that.


Folkert van Heusden
