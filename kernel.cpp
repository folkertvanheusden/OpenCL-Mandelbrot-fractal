// written by Folkert van Heusden
// released under MIT license

#include "OpenCL-Wrapper/src/kernel.hpp"

string opencl_c_container()
{
	return R(
			kernel void mandelbrot_kernel(global uint *dims, global float *coordinates, global uint *out)
			{
				const uint  n      = get_global_id(0);
				const uint  x_int  = n % dims[0];
				const uint  y_int  = n / dims[0];
				const double re     = (coordinates[2] - coordinates[0]) / dims[0] * x_int + coordinates[0];
				const double im     = (coordinates[3] - coordinates[1]) / dims[1] * y_int + coordinates[1];

				uint        cur_it = 0;
				double      x      = 0;
				double      y      = 0;
				while(++cur_it < dims[2]) {
					double dx = x * x;
					double dy = y * y;
					if (dx + dy >= 4.0)
						break;
					double tx = dx - dy + re;
					y = 2 * x * y + im;
					x = tx;
				}

				if (cur_it < dims[2]) {
					int offset = n * 3;
					out[offset + 0] = x * x;
					out[offset + 1] = cur_it;
					out[offset + 2] = y * y;
				}
			}
		);
}
