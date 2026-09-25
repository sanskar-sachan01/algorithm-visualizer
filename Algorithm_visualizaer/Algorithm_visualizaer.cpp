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

int main(int argc, char* argv[])
{
    std::random_device rd;
    std::uniform_int_distribution d(1, 99);

    std::vector<int> v;

    
    for (int i = 0; i < 100; i++)
    {
        v.push_back(d(rd));
    }

    
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    SDL_CreateWindowAndRenderer(
        100 * 10,
        100 * 10,
        0,
        &window,
        &renderer
    );

    SDL_RenderSetScale(renderer, 10, 10);

    
    for (unsigned int i = 0; i < v.size(); i++)
    {
        for (unsigned int j = i; j < v.size(); j++)
        {
            
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            
            draw_state(v, renderer, i, j, i);

            SDL_RenderPresent(renderer);
            SDL_Delay(15);

            
            if (v[j] < v[i])
            {
                std::swap(v[j], v[i]);
            }
        }
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    draw_state(v, renderer, 100, 100, 100);

    SDL_RenderPresent(renderer);

    std::cout << "Sorted: "
        << std::ranges::is_sorted(v)
        << '\n';

    SDL_Delay(2000);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}