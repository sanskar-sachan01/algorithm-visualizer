#include <algorithm>
#include <iostream>
#include <limits>
#include <random>
#include <ranges>
#include <vector>

#include <SDL.h>

#include "algorithms/bubble_sort.h"
#include "algorithms/insertion_sort.h"
#include "algorithms/merge_sort.h"
#include "algorithms/quick_sort.h"
#include "algorithms/selection_sort.h"
#include "algorithms/visualization.h"

void draw_state(
    const std::vector<int>& values,
    SDL_Renderer* renderer,
    unsigned int red,
    unsigned int blue,
    unsigned int sorted
)
{
    for (int index = 0; index < static_cast<int>(values.size()); index++)
    {
        if (index < static_cast<int>(sorted))
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        else if (index == static_cast<int>(red))
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        else if (index == static_cast<int>(blue))
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        else
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        SDL_RenderDrawLine(
            renderer,
            index,
            99,
            index,
            values[index]
        );
    }
}

bool visualize_step(
    const std::vector<int>& values,
    SDL_Renderer* renderer,
    unsigned int red,
    unsigned int blue
)
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
            return false;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    draw_state(values, renderer, red, blue, 0);
    SDL_RenderPresent(renderer);
    SDL_Delay(15);

    return true;
}

int choose_algorithm()
{
    int choice = 0;

    while (choice < 1 || choice > 5)
    {
        std::cout << "Choose a sorting algorithm:\n"
                  << "1. Bubble Sort\n"
                  << "2. Selection Sort\n"
                  << "3. Insertion Sort\n"
                  << "4. Merge Sort\n"
                  << "5. Quick Sort\n"
                  << "> ";

        if (std::cin >> choice && choice >= 1 && choice <= 5)
            return choice;

        std::cout << "Please enter a number from 1 to 5.\n\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    return choice;
}

int main()
{
    const int value_count = 100;
    const int window_size = value_count * 10;
    const int algorithm_choice = choose_algorithm();

    std::random_device random_device;
    std::uniform_int_distribution<int> distribution(1, 99);
    std::vector<int> values;

    for (int index = 0; index < value_count; index++)
        values.push_back(distribution(random_device));

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cerr << "SDL_Init failed: "
                  << SDL_GetError() << '\n';
        return 1;
    }

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    if (SDL_CreateWindowAndRenderer(
            window_size,
            window_size,
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

    bool sorted_successfully = true;

    switch (algorithm_choice)
    {
    case 1:
        sorted_successfully = bubble_sort(values, renderer);
        break;
    case 2:
        sorted_successfully = selection_sort(values, renderer);
        break;
    case 3:
        sorted_successfully = insertion_sort(values, renderer);
        break;
    case 4:
        sorted_successfully = merge_sort(values, renderer);
        break;
    case 5:
        sorted_successfully = quick_sort(values, renderer);
        break;
    }

    if (!sorted_successfully)
    {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 0;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    draw_state(values, renderer, value_count, value_count, value_count);
    SDL_RenderPresent(renderer);

    std::cout << "Sorted: "
              << std::ranges::is_sorted(values)
              << '\n';

    bool running = true;
    SDL_Event event;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                running = false;
        }

        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
