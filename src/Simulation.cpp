/*
CPP translation of original Objective-C MetalAdder.m. Some stuff has been moved
over to the header. Source:
https://developer.apple.com/documentation/metal/performing_calculations_on_a_gpu?language=objc

Original distribution license: LICENSE-original.txt.

Abstract:
A class to manage all of the Metal objects this app creates.
*/

#include "Simulation.hpp"
#include "config.h"
#include <cstdlib>
#include <iostream>

Simulation::Simulation(MTL::Device *device) {

    _mDevice = device;

    NS::Error *error = nullptr;

    NS::String *path =
        NS::String::string("shaders.metallib", NS::UTF8StringEncoding);
    MTL::Library *defaultLibrary = device->newLibrary(path, &error);

    auto str = NS::String::string("reactionDiffusion", NS::ASCIIStringEncoding);
    MTL::Function *addFunction = defaultLibrary->newFunction(str);
    defaultLibrary->release();

    if (addFunction == nullptr) {
        std::cout << "Failed to find the adder function." << std::endl;
        return;
    }

    // Create a compute pipeline state object.
    _mAddFunctionPSO = _mDevice->newComputePipelineState(addFunction, &error);
    addFunction->release();

    if (_mAddFunctionPSO == nullptr) {
        //  If the Metal API validation is enabled, you can find out more
        //  information about what went wrong.  (Metal API validation is enabled
        //  by default when a debug build is run from Xcode)
        std::cout << "Failed to created pipeline state object, error " << error
                  << "." << std::endl;
        return;
    }

    _mCommandQueue = _mDevice->newCommandQueue();
    if (_mCommandQueue == nullptr) {
        std::cout << "Failed to find the command queue." << std::endl;
        return;
    }

    // Allocate three buffers to hold our initial data and the result.
    imgBuffer =
        _mDevice->newBuffer(WIDTH * HEIGHT * 3, MTL::ResourceStorageModeShared);
    data1 = _mDevice->newBuffer(WIDTH * HEIGHT * sizeof(float) * 3,
                                MTL::ResourceStorageModeShared);
    data2 = _mDevice->newBuffer(WIDTH * HEIGHT * sizeof(float) * 3,
                                MTL::ResourceStorageModeShared);

    float *content = (float *)data1->contents();
    for (int i = 0; i < HEIGHT * WIDTH; i += 1) {
        content[3 * i] = 1.;
        content[3 * i + 1] = 0.;
        content[3 * i + 2] = 0.;
    }
    for (int x = 0; x < 100; x += 1) {
        for (int y = 0; y < 100; y += 1) {
            int pos = (x + WIDTH/2 - 50) + (y + HEIGHT/2 - 50) * WIDTH;
            content[3 * pos] = 0.;
            content[3 * pos + 1] = 1.;
        }
    }
}

void Simulation::step() {
    // Create a command buffer to hold commands.
    MTL::CommandBuffer *commandBuffer = _mCommandQueue->commandBuffer();
    assert(commandBuffer != nullptr);

    // Start a compute pass.
    MTL::ComputeCommandEncoder *computeEncoder =
        commandBuffer->computeCommandEncoder();
    assert(computeEncoder != nullptr);

    encodeStepCommand(computeEncoder);

    // End the compute pass.
    computeEncoder->endEncoding();

    // Execute the command.
    commandBuffer->commit();

    // Normally, you want to do other work in your app while the GPU is running,
    // but in this example, the code simply blocks until the calculation is
    // complete.
    commandBuffer->waitUntilCompleted();
    // std::cout << (unsigned long)content[0] << std::endl;

    MTL::Buffer *tmp = data1;
    data1 = data2;
    data2 = tmp;
}

void Simulation::encodeStepCommand(MTL::ComputeCommandEncoder *computeEncoder) {
    // Encode the pipeline state object and its parameters.
    computeEncoder->setComputePipelineState(_mAddFunctionPSO);
    computeEncoder->setBuffer(data1, 0, 0);
    computeEncoder->setBuffer(data2, 0, 1);
    computeEncoder->setBuffer(imgBuffer, 0, 2);

    MTL::Size gridSize = MTL::Size::Make(WIDTH * HEIGHT, 1, 1);

    // Calculate a threadgroup size.
    NS::UInteger threadGroupSize =
        _mAddFunctionPSO->maxTotalThreadsPerThreadgroup();
    if (threadGroupSize > WIDTH * HEIGHT) {
        threadGroupSize = WIDTH * HEIGHT;
    }
    MTL::Size threadgroupSize = MTL::Size::Make(threadGroupSize, 1, 1);

    // Encode the compute command.
    computeEncoder->dispatchThreads(gridSize, threadgroupSize);
}

Simulation::~Simulation() {
    data1->release();
    data2->release();
    imgBuffer->release();

    _mAddFunctionPSO->release();
    _mCommandQueue->release();
}
