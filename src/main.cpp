#define NS_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION

#include "Display.hpp"
#include "Simulation.hpp"
int main(int argc, char *argv[]) {
    Display display = Display();
    MTL::Device *device = MTL::CreateSystemDefaultDevice();

    Simulation simulation = Simulation(device);

    bool keep_window_open = true;
    while (keep_window_open) {
        SDL_Event e;
        simulation.step();
        display.fill((uint8_t *)simulation._mBufferA->contents());
        SDL_Delay(10);
        while (SDL_PollEvent(&e) > 0) {
            switch (e.type) {
            case SDL_QUIT:
                keep_window_open = false;
                break;
            }
        }
    }

    device->release();
}
