#include <iostream>
#include <SDL.h>
#include <ranges>
#include <random>
#include <algorithm>
#include <vector>

void draw_state(
    const std::vector<int>& v,
    SDL_Renderer* renderer,
    unsigned int red,
    unsigned int blue,
    unsigned int sorted
)
{
    for (int index = 0; index < v.size(); index++)
    {
        if (index < sorted)
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        else if (index == red)
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        else if (index == blue)
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        else
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        SDL_RenderDrawLine(
            renderer,
            index,
            99,
            index,
            v[index]
        );
    }
}

bool bubble_sort(std::vector<int>& v, SDL_Renderer* renderer)
{
    for (unsigned int i = 0; i < v.size(); i++)
    {
        for (unsigned int j = 0; j + 1 < v.size() - i; j++)
        {
            SDL_Event event;

            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT)
                    return false;
            }

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            draw_state(v, renderer, j, j + 1, 0);

            SDL_RenderPresent(renderer);
            SDL_Delay(15);

            if (v[j] > v[j + 1])
                std::swap(v[j], v[j + 1]);
        }
    }

    return true;
}

int main(int argc, char* argv[])
{
    // Generate random values
    std::random_device rd;
    std::uniform_int_distribution<int> d(1, 99);

    std::vector<int> v;

    for (int i = 0; i < 100; i++)
    {
        v.push_back(d(rd));
    }

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cerr << "SDL_Init failed: "
                  << SDL_GetError() << '\n';
        return 1;
    }

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    if (SDL_CreateWindowAndRenderer(
            100 * 10,
            100 * 10,
            0,
            &window,
            &renderer) != 0)
    {
        std::cerr << "SDL_CreateWindowAndRenderer failed: "
                  << SDL_GetError() << '\n';

        SDL_Quit();
        return 1;
    }

    SDL_RenderSetScale(renderer, 10, 10);

    if (!bubble_sort(v, renderer))
    {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 0;
    }

    // Draw final sorted array
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    draw_state(v, renderer, 100, 100, 100);

    SDL_RenderPresent(renderer);

    std::cout << "Sorted: "
              << std::ranges::is_sorted(v)
              << '\n';

    // Keep window open until user closes it
    bool running = true;
    SDL_Event event;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
        }

        SDL_Delay(16);
    }

    // Cleanup
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}