// written by Folkert van Heusden
// released under MIT license

#include <chrono>
#include <cstdio>

#include "OpenCL-Wrapper/src/opencl.hpp"

int main(int argc, char *argv[])
{
	FILE *fh = fopen("test.ppm", "w");
	if (!fh) {
		fprintf(stderr, "Cannot create output file\n");
		return 1;
	}

	Device device(select_device_with_most_flops());

	Memory<uint> dimensions(device, 3);
	dimensions[0] = 3840;  // w
	dimensions[1] = 2160;  // h
	dimensions[2] = 256;  // it
	dimensions.write_to_device();

	Memory<float> coordinates(device, 4);
	coordinates[0] = -2.f;
	coordinates[1] = -2.f;
	coordinates[2] =  2.f;
	coordinates[3] =  2.f;
	coordinates.write_to_device();

	const uint   N   = dimensions[0] * dimensions[1];
	Memory<uint> output(device, N * 3); // allocate memory on both host and device

	Kernel mandelbrot(device, N, "mandelbrot_kernel", dimensions, coordinates, output);

        auto start_ts = std::chrono::system_clock::now();
	mandelbrot.run();
        auto end_ts = std::chrono::system_clock::now();

	printf("Calculation took %lu microseconds\n", std::chrono::duration_cast<std::chrono::microseconds>(end_ts - start_ts).count());

	output.read_from_device();

	uint max_val = 0;
	for(uint i=0; i<N * 3; i++)
		max_val = std::max(max_val, output[i]);

	fprintf(fh, "P3\n%d\n%d\n%d\n", dimensions[0], dimensions[1], max_val);
	for(uint i=0; i<N * 3; i++)
		fprintf(fh, "%d%c", output[i], (i % 7) == 0 ? '\n' : ' ');
	fprintf(fh, "\n");
	fclose(fh);

	return 0;
}
