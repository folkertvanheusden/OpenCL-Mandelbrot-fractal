#include "OpenCL-Wrapper/src/kernel.hpp"

string opencl_c_container()
{
	return R(
			kernel void mandelbrot_kernel(global int *wh, global int *m_it, global float *coordinates, global uint *out)
			{
				const uint  n      = get_global_id(0);
				const uint  x_int  = n % wh[0];
				const uint  y_int  = n / wh[0];
				const float re     = (coordinates[2] - coordinates[0]) / wh[0] * x_int + coordinates[0];
				const float im     = (coordinates[3] - coordinates[1]) / wh[1] * y_int + coordinates[1];

				uint        cur_it = 0;
				float       x      = 0;
				float       y      = 0;
				while(++cur_it < *m_it) {
					float dx = x * x;
					float dy = y * y;
					if (dx + dy > 4.0)
						break;
					float tx = dx - dy + re;
					y = 2 * x * y + im;
					x = tx;
				}

				out[n] = cur_it == *m_it ? 0 : cur_it;
			}
		);
}
