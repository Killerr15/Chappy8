#pragma once

#include <iostream>
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <thread>
#include <string>
#include <memory>
#include "chip8.hpp"

void audioCallback(void* userdata, uint8_t* stream, int len);

SDL_AudioSpec want{ .freq = 44100, .format = AUDIO_S16SYS, .channels = 1, .samples = 4096, .callback = audioCallback }, have;