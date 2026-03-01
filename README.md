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

After a while a `test.avi` file is created. vlc displays this.
Most time is in writing the avi. On a Raspberry Pi 5 with a Geforce RTX 3060 card, the calculation takes around 18 seconds.


Folkert van Heusden
