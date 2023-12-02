#include "config.h"
#include <metal_stdlib>

#define case(x, y) (3 * ((x+WIDTH)%WIDTH + WIDTH*((y+WIDTH)%HEIGHT)))

#define inside(x, y) (x >= 0 && x < WIDTH-1 && y >= 0 && y < HEIGHT-1)

#define isColored(x, y, data) (data[case (x, y)] > 128)

using namespace metal;
kernel void simulate(device const uint8_t *imgIn, device uint8_t *imgOut,
                     uint index [[thread_position_in_grid]]) {
    uint x0 = index % WIDTH;
    uint y0 = index / WIDTH;
    uint colI = case(x0,y0);

    uint8_t nVoisins = 0;
    nVoisins += isColored(x0 + 1, y0, imgIn);
    nVoisins += isColored(x0 + 1, y0 + 1, imgIn);
    nVoisins += isColored(x0, y0 + 1, imgIn);
    nVoisins += isColored(x0 - 1, y0 + 1, imgIn);
    nVoisins += isColored(x0 - 1, y0, imgIn);
    nVoisins += isColored(x0 - 1, y0 - 1, imgIn);
    nVoisins += isColored(x0, y0 - 1, imgIn);
    nVoisins += isColored(x0 + 1, y0 - 1, imgIn);

    if (nVoisins == 3) {
		if(imgIn[colI]){
			imgOut[colI+1] = min(255, imgOut[colI+1]+1);
		}
        imgOut[colI] = 255;
    } else if (nVoisins == 2) {
		if(imgIn[colI]){
			imgOut[colI+1] = min(255, imgOut[colI+1]+1);
		}
        imgOut[colI] = imgIn[colI];
    }
	else{
        imgOut[colI] = 0;
		imgOut[colI+1] = 0;
	}

	int cx=x0-WIDTH/2;
	int cy=y0-HEIGHT/2;
	if(abs(cx)-2 < 0 && abs(cy) - 300 < 0){
		imgOut[case(x0, y0)] = 255;
	}
	
    // imgOut[colI] = (imgIn[case(x0, y0)]+imgIn[case(x0-1, y0)])/2 ;
}
