#pragma once

#include <vector>

struct SDL_Renderer;

void draw_state(
    const std::vector<int>& values,
    SDL_Renderer* renderer,
    unsigned int red,
    unsigned int blue,
    unsigned int sorted
);

bool visualize_step(
    const std::vector<int>& values,
    SDL_Renderer* renderer,
    unsigned int red,
    unsigned int blue
);
