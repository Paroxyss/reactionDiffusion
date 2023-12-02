/*
CPP translation of original Objective-C MetalAdder.m. Some stuff has been moved over to
the header. Source: https://developer.apple.com/documentation/metal/performing_calculations_on_a_gpu?language=objc

Original distribution license: LICENSE-original.txt.

Abstract:
A class to manage all of the Metal objects this app creates.
*/

#include "Simulation.hpp"
#include "config.h"
#include <cstdlib>
#include <iostream>

Simulation::Simulation(MTL::Device *device)
{

    _mDevice = device;

    NS::Error *error = nullptr;

    NS::String *path =
        NS::String::string("shaders.metallib", NS::UTF8StringEncoding);
    MTL::Library *defaultLibrary = device->newLibrary(path, &error);
	
    auto str = NS::String::string("simulate", NS::ASCIIStringEncoding);
    MTL::Function *addFunction = defaultLibrary->newFunction(str);
    defaultLibrary->release();

    if (addFunction == nullptr)
    {
        std::cout << "Failed to find the adder function." << std::endl;
        return;
    }

    // Create a compute pipeline state object.
    _mAddFunctionPSO = _mDevice->newComputePipelineState(addFunction, &error);
    addFunction->release();

    if (_mAddFunctionPSO == nullptr)
    {
        //  If the Metal API validation is enabled, you can find out more information about what
        //  went wrong.  (Metal API validation is enabled by default when a debug build is run
        //  from Xcode)
        std::cout << "Failed to created pipeline state object, error " << error << "." << std::endl;
        return;
    }

    _mCommandQueue = _mDevice->newCommandQueue();
    if (_mCommandQueue == nullptr)
    {
        std::cout << "Failed to find the command queue." << std::endl;
        return;
    }

    // Allocate three buffers to hold our initial data and the result.
    _mBufferA = _mDevice->newBuffer(WIDTH*HEIGHT*3, MTL::ResourceStorageModeShared);
    _mBufferB = _mDevice->newBuffer(WIDTH*HEIGHT*3, MTL::ResourceStorageModeShared);

	auto content = (uint8_t*) _mBufferA->contents();
	for(int i = 0; i < WIDTH*HEIGHT*3; i+=3){
		//content[i] = (rand() < __INT_MAX__/2)*255;	
	}
}

void Simulation::step()
{
    // Create a command buffer to hold commands.
    MTL::CommandBuffer *commandBuffer = _mCommandQueue->commandBuffer();
    assert(commandBuffer != nullptr);

    // Start a compute pass.
    MTL::ComputeCommandEncoder *computeEncoder = commandBuffer->computeCommandEncoder();
    assert(computeEncoder != nullptr);

    encodeStepCommand(computeEncoder);

    // End the compute pass.
    computeEncoder->endEncoding();

    // Execute the command.
    commandBuffer->commit();

    // Normally, you want to do other work in your app while the GPU is running,
    // but in this example, the code simply blocks until the calculation is complete.
    commandBuffer->waitUntilCompleted();
	auto content = (uint8_t*) _mBufferB->contents();
	//std::cout << (unsigned long)content[0] << std::endl;
	
    MTL::Buffer *tmp = _mBufferA;
    _mBufferA = _mBufferB;
	_mBufferB = tmp;
}

void Simulation::encodeStepCommand(MTL::ComputeCommandEncoder *computeEncoder)
{
    // Encode the pipeline state object and its parameters.
    computeEncoder->setComputePipelineState(_mAddFunctionPSO);
    computeEncoder->setBuffer(_mBufferA, 0, 0);
    computeEncoder->setBuffer(_mBufferB, 0, 1);

    MTL::Size gridSize = MTL::Size::Make(WIDTH*HEIGHT, 1, 1);

    // Calculate a threadgroup size.
    NS::UInteger threadGroupSize = _mAddFunctionPSO->maxTotalThreadsPerThreadgroup();
    if (threadGroupSize > WIDTH*HEIGHT)
    {
        threadGroupSize = WIDTH*HEIGHT;
    }
    MTL::Size threadgroupSize = MTL::Size::Make(threadGroupSize, 1, 1);

    // Encode the compute command.
    computeEncoder->dispatchThreads(gridSize, threadgroupSize);
}

Simulation::~Simulation()
{
    _mBufferA->release();
    _mBufferB->release();

    _mAddFunctionPSO->release();
    _mCommandQueue->release();
}
