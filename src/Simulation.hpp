/*
CPP translation of original Objective-C MetalAdder.h. Some stuff has been moved
over here from the cpp file. Source:
https://developer.apple.com/documentation/metal/performing_calculations_on_a_gpu?language=objc

Original distribution license: LICENSE-original.txt.

Abstract:
A class to manage all of the Metal objects this app creates.
*/
#pragma once
#include "config.h"
#include <Metal/Metal.hpp>

extern MTL::Buffer *parameters;

class Simulation {
  public:
    MTL::Device *_mDevice;

    // The compute pipeline generated from the compute kernel in the .metal
    // shader file.
    MTL::ComputePipelineState *_mAddFunctionPSO;

    // The command queue used to pass commands to the device.
    MTL::CommandQueue *_mCommandQueue;

    // Buffers to hold data.
    MTL::Buffer *imgBuffer;
    MTL::Buffer *data1;
    MTL::Buffer *data2;

    Simulation(MTL::Device *device);
    ~Simulation();

	void init();
	void addB(int x, int y);
    void step();
    static void delta(float ddiffA, float ddiffB, float dfeed, float dkill);

  private:
    void encodeStepCommand(MTL::ComputeCommandEncoder *computeEncoder);
};
