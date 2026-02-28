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
	dimensions[0] = 4096;  // w
	dimensions[1] = 4096;  // h
	dimensions[2] = 80;  // it
	dimensions.write_to_device();

	Memory<double> coordinates(device, 4);
	coordinates[0] = -2.;
	coordinates[1] = -2.;
	coordinates[2] =  2.;
	coordinates[3] =  2.;
	coordinates.write_to_device();

	const uint   N   = dimensions[0] * dimensions[1];
	Memory<uint> output(device, N * 3); // allocate memory on both host and device

	Kernel mandelbrot(device, N, "mandelbrot_kernel", dimensions, coordinates, output);

        auto start_ts = std::chrono::system_clock::now();
	mandelbrot.run();
        auto end_ts = std::chrono::system_clock::now();

	printf("Calculation took %lu microseconds\n", std::chrono::duration_cast<std::chrono::microseconds>(end_ts - start_ts).count());

	output.read_from_device();

	uint max_rb = 0;
	for(uint i=0; i<N * 3; i += 3) {
		max_rb = std::max(max_rb, output[i + 0]);
		max_rb = std::max(max_rb, output[i + 2]);
	}

	printf("%d\n", max_rb);

	fprintf(fh, "P3\n%d\n%d\n%d\n", dimensions[0], dimensions[1], dimensions[2]);
	for(uint i=0; i<N * 3; i += 3) {
		fprintf(fh, "%d ", output[i + 0] * dimensions[2] / max_rb);
		fprintf(fh, "%d ", output[i + 1] * 255 / dimensions[2]);
		fprintf(fh, "%d%c", output[i + 2] * dimensions[2] / max_rb, (i % 21) == 0 ? '\n' : ' ');
	}
	fprintf(fh, "\n");
	fclose(fh);

	return 0;
}
