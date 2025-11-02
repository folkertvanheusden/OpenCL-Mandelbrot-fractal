#include <stdio.h>

#include "OpenCL-Wrapper/src/opencl.hpp"


int main(int argc, char *argv[])
{
	Device device(select_device_with_most_flops());

	Memory<uint> dimensions(device, 3);
	dimensions[0] = 256;  // w
	dimensions[1] = 256;  // h
	dimensions[1] = 16;  // it
	dimensions.write_to_device();

	Memory<float> coordinates(device, 4);
	coordinates[0] = -2.f;
	coordinates[1] = -2.f;
	coordinates[2] =  2.f;
	coordinates[3] =  2.f;
	coordinates.write_to_device();

	const uint   N   = dimensions[0] * dimensions[1];
	Memory<uint> output(device, N); // allocate memory on both host and device

	Kernel mandelbrot(device, N, "mandelbrot_kernel", dimensions, coordinates, output);
	mandelbrot.run();

	output.read_from_device();

	wait();

	return 0;
}
