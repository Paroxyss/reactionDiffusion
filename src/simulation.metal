#include "config.h"
#include <metal_stdlib>

#ifdef debug
#error

#define kernel
#define device
#define thread_position_in_grid
typedef int unsigned uint;
typedef int unsigned uint_8;
float min(float a, float b){};
namespace metal {}
#endif

#define case(x, y) (3 * ((x + WIDTH) % WIDTH + WIDTH * ((y + WIDTH) % HEIGHT)))

#define inside(x, y) (x >= 0 && x < WIDTH - 1 && y >= 0 && y < HEIGHT - 1)

#define isColored(x, y, data) (data[case (x, y)] > 128)

using namespace metal;

kernel void reactionDiffusion(device const float *cIn, device float *cOut,
                              device uint8_t *imgOut,
                              uint index [[thread_position_in_grid]]) {
    uint x0 = index % WIDTH;
    uint y0 = index / WIDTH;
    uint colI = case(x0,y0);

	// source : https://www.karlsims.com/rd.html
	// A' = A + (Da(∇2)A - AB2 + f(1-A))*dt
	// B' = B + (Db(∇2)B + AB2 - (k+f)B)*dt
	 
	float A = cIn[colI];
	float B = cIn[colI+1];

	float da;
	float Aprime;
	float db;
	float Bprime;
	
	const float weights[3] = {-1, 0.2, 0.05};
	float laplacienA = 0;
	float laplacienB = 0;
	
	for(int dx = -1; dx < 2; dx++){
		for(int dy = -1; dy < 2; dy++){
			laplacienA += cIn[case(x0+dx, y0+dy)] * weights[abs(dx) + abs(dy)];
			laplacienB += cIn[case(x0+dx, y0+dy)+1] * weights[abs(dx) + abs(dy)];
		}
	}
	float cx = x0 - WIDTH /2.;
	float cy = y0 - HEIGHT/2.;
	//float distanceFromCenter = sqrt((float)(cx*cx + cy*cy));
	
	//float killRate = (killRateB - killRateA)*(distanceFromCenter/2000.) + killRateA;
	da = (laplacienA*diffusionA - A*B*B + feedRate*(1-A))*dt;
	Aprime = A + da;
	db = (laplacienB*diffusionB + A*B*B - (killRate+feedRate)*B)*dt;
	Bprime = B + db;

	cOut[colI] = Aprime;
	cOut[colI+1] = Bprime;

	imgOut[colI] = min(255., max(max(0.,db*1000000), Bprime*712));
	imgOut[colI+1] = min(255., Bprime*712);
	imgOut[colI+2] = min(255., max(max(0.,-db*1000000), Bprime*712));
	
}
