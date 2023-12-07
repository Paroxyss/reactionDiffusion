#define NS_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION

#include "Display.hpp"
#include "Simulation.hpp"

void handleKey(int key, Simulation &sim) {
    switch (key) {
    case SDLK_a:
        Simulation::delta(0.01, 0, 0, 0);
        break;
    case SDLK_q:
        Simulation::delta(-0.01, 0, 0, 0);
        break;
    case SDLK_z:
        Simulation::delta(0, 0.01, 0, 0);
        break;
    case SDLK_s:
        Simulation::delta(0, -0.01, 0, 0);
        break;
    case SDLK_e:
        Simulation::delta(0, 0, 0.001, 0);
        break;
    case SDLK_d:
        Simulation::delta(0, 0, -0.001, 0);
        break;
    case SDLK_r:
        Simulation::delta(0, 0, 0, 0.001);
        break;
    case SDLK_f:
        Simulation::delta(0, 0, 0, -0.001);
        break;
    case SDLK_SPACE:
        sim.init();
        break;
    }
}

int main(int argc, char *argv[]) {
    Display display = Display();
    MTL::Device *device = MTL::CreateSystemDefaultDevice();

    Simulation simulation = Simulation(device);

    bool keep_window_open = true;
    while (keep_window_open) {
        SDL_Event e;
        for (int i = 0; i < 80; i++) {
            simulation.step();
        }
        display.fill((uint8_t *)simulation.imgBuffer->contents());
        SDL_Delay(10);
        while (SDL_PollEvent(&e) > 0) {
            switch (e.type) {
            case SDL_QUIT:
                keep_window_open = false;
                break;
            case SDL_KEYDOWN:
                handleKey(e.key.keysym.sym, simulation);
				break;
			case SDL_MOUSEBUTTONDOWN:
				simulation.addB(e.button.x, e.button.y);
            }
        }
    }

    device->release();
}
