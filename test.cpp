// written by Folkert van Heusden
// released under MIT license

#include <chrono>
#include <cstdio>
extern "C" {
#include <gwavi.h>
}
#include <turbojpeg.h>

#include "OpenCL-Wrapper/src/opencl.hpp"


int main(int argc, char *argv[])
{
	tjhandle jpeg_compressor = tjInitCompress();

	Device device(select_device_with_most_flops());

	Memory<uint> dimensions(device, 3);
	dimensions[0] = 1080;  // w
	dimensions[1] = 1080;  // h
	dimensions[2] = 256;  // it
	dimensions.write_to_device();

	double start_x1 = -2;
	double start_x2 =  2;
	double start_y1 = -2;
	double start_y2 =  2;

	double end_x1   =  -0.812223315621338 - 0.0000000001;
	double end_x2   =  -0.812223315621338 + 0.0000000001;
	double end_y1   =  -0.185453926110785 - 0.0000000001;
	double end_y2   =  -0.185453926110785 + 0.0000000001;

	int n_steps = 250;

	Memory<double> coordinates(device, 4);

	const uint N = dimensions[0] * dimensions[1];
	Memory<uint> output(device, N * 3); // allocate memory on both host and device

	Kernel mandelbrot(device, N, "mandelbrot_kernel", dimensions, coordinates, output);

	int fps = 25;
	gwavi_t *gwavi  = gwavi_open("test.avi", dimensions[0], dimensions[1], "MJPG", fps, nullptr);

	uint8_t *out_8b = new uint8_t[dimensions[0] * dimensions[1] * 3];

	for(int step=n_steps; step>0; step--) {
		double step_factor = step / double(n_steps);
		coordinates[0] = (end_x1 - start_x1) * step_factor + start_x1;
		coordinates[1] = (end_x2 - start_x2) * step_factor + start_x2;
		coordinates[2] = (end_y1 - start_y1) * step_factor + start_y1;
		coordinates[3] = (end_y2 - start_y2) * step_factor + start_y2;
		coordinates.write_to_device();

		auto start_ts = std::chrono::system_clock::now();
		mandelbrot.run();
		auto end_ts = std::chrono::system_clock::now();

		printf("%d/%d] Calculation took %lu microseconds\n", step + 1, n_steps, std::chrono::duration_cast<std::chrono::microseconds>(end_ts - start_ts).count());

		output.read_from_device();

		uint max_rb = 0;
		for(uint i=0; i<N * 3; i += 3) {
			max_rb = std::max(max_rb, output[i + 0]);
			max_rb = std::max(max_rb, output[i + 2]);
		}

		for(uint i=0; i<N * 3; i += 3) {
			output[i + 0] = output[i + 0] * dimensions[2] / max_rb;
			output[i + 1] = output[i + 1] * 255 / dimensions[2];
			output[i + 2] = output[i + 2] * dimensions[2] / max_rb;
		}

		for(uint i=0; i<N * 3; i++)
			out_8b[i] = output[i];

		uint8_t          *temp    = NULL;
		unsigned long int len     = 0;
		int               quality = 75;
		if (tjCompress2(jpeg_compressor, out_8b, dimensions[0], 0, dimensions[1], TJPF_RGB, &temp, &len, TJSAMP_444, quality, TJFLAG_FASTDCT) == -1) {
			fprintf(stderr, "Failed compressing frame: %s", tjGetErrorStr());
			return 1;
		}

		gwavi_add_frame(gwavi, temp, len);

		free(temp);
	}

	delete [] out_8b;

	gwavi_close(gwavi);

	tjDestroy(jpeg_compressor);

	return 0;
}
