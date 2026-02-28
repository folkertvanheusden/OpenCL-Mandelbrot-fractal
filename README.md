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

After a while (initialization takes by far the longest) a `test.ppm` file is created. Most Linux picture viewers can open that.
Most time is in writing the PPM. On a Raspberry Pi 5 with a Geforce RTX 3060 card, the calculation takes less than 23ms.


Folkert van Heusden
