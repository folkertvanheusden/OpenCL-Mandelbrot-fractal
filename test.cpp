#include <stdio.h>

#include "OpenCL-Wrapper/src/opencl.hpp"


int main(int argc, char *argv[])
{
	Device device(select_device_with_most_flops());

	Memory<uint> wh(device, 2);
	wh[0] = 256;
	wh[1] = 256;
	wh.write_to_device();

	Memory<uint> m_it(device, 1);
	m_it[0] = 256;
	m_it.write_to_device();

	Memory<float> coordinates(device, 4);
	coordinates[0] = -2.f;
	coordinates[1] = -2.f;
	coordinates[2] =  2.f;
	coordinates[3] =  2.f;
	coordinates.write_to_device();

	const uint   N   = wh[0] * wh[1];
	Memory<uint> output(device, N); // allocate memory on both host and device

	Kernel mandelbrot(device, N, "mandelbrot_kernel", wh, m_it, coordinates, output);
	mandelbrot.run();

	output.read_from_device();

	wait();

	return 0;
}
