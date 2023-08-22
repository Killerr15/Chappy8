#include "Chappy8.hpp"

uint8_t keymap[16] = {
		SDLK_x,
		SDLK_1,
		SDLK_2,
		SDLK_3,
		SDLK_q,
		SDLK_w,
		SDLK_e,
		SDLK_a,
		SDLK_s,
		SDLK_d,
		SDLK_z,
		SDLK_c,
		SDLK_4,
		SDLK_r,
		SDLK_f,
		SDLK_v,
};

int main(int argc, char* argv[])
{

	if (argc == 1)
	{
		std::cerr << "ERROR: Specify the path to the ROM file." << std::endl;
		std::cin.get();
		exit(1);
	}

	constexpr int cycleDelay = 10;

	auto lastCycleTime = std::chrono::high_resolution_clock::now();

	chip8 cpu;

	if (!cpu.loadROM(argv[1]))
	{
		exit(0);
	}

	SDL_Init(SDL_INIT_EVERYTHING);
	auto window = std::shared_ptr<SDL_Window>(SDL_CreateWindow("Chappy++", 100, 100, 1280, 720, NULL), SDL_DestroyWindow);
	if (window == nullptr)
	{
		std::cout << "Oops! Couldn't create SDL window! " << SDL_GetError() << std::endl;

		exit(1);
	}

	auto renderer = std::shared_ptr<SDL_Renderer>(SDL_CreateRenderer(window.get(), NULL, SDL_RENDERER_ACCELERATED), SDL_DestroyRenderer);
	if (renderer == nullptr)
	{
		std::cout << "Oops! Couldn't create SDL renderer!" << SDL_GetError() << std::endl;

		exit(1);
	}

	auto tex = std::shared_ptr<SDL_Texture>(SDL_CreateTexture(renderer.get(), SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, 64, 32), SDL_DestroyTexture);
	if (tex == nullptr)
	{
		std::cerr << "Oops! Couldn't create SDL texture!" << SDL_GetError() << std::endl;
		exit(1);
	}

	while (1)
	{

		auto currentTime = std::chrono::high_resolution_clock::now();
		float dt = std::chrono::duration<float, std::chrono::milliseconds::period>(currentTime - lastCycleTime).count();
		
		SDL_Event event;

		if (SDL_PollEvent(&event) >= 0)
		{
			switch (event.type)
			{
			case SDL_QUIT:
				exit(0);
			case (SDL_KEYDOWN):
				if (event.key.keysym.sym == SDLK_ESCAPE)
				{
					exit(0);
				}

				for (int i = 0; i < 16; ++i)
				{
					if (event.key.keysym.sym == keymap[i])
					{
						cpu.setKeyValue(i, 1);
					}
				}
				break;
			case (SDL_KEYUP):
				for (int i = 0; i < 16; ++i)
				{
					if (event.key.keysym.sym == keymap[i])
					{
						cpu.setKeyValue(i, 0);
					}
				}
				break;
			default:
				break;
			}
		}

		if (dt > cycleDelay)
		{
			lastCycleTime = currentTime;
			cpu.emulate();
		}

		if (cpu.getDrawFlag() == true)
		{
			cpu.setDrawFlag(false);
			uint32_t pixels[32 * 64];

			for (int i = 0; i < 32 * 64; i++)
			{
				if (cpu.getDisplayVal(i) == 0)
				{
					pixels[i] = 0xFF000000;
				}
				else
				{
					pixels[i] = 0xFFFFFFFF;
				}

			}

			SDL_UpdateTexture(tex.get(), NULL, pixels, 64 * sizeof(uint32_t));
			SDL_RenderClear(renderer.get());
			SDL_RenderCopy(renderer.get(), tex.get(), NULL, NULL);
			SDL_RenderPresent(renderer.get());
		}
	}
}
