#include "bubble_sort.h"

#include <SDL.h>
#include <algorithm>

#include "visualization.h"

bool bubble_sort(std::vector<int>& v, SDL_Renderer* renderer)
{
    for (unsigned int end = v.size(); end > 1; end--)
    {
        for (unsigned int current = 0; current + 1 < end; current++)
        {
            if (!visualize_step(v, renderer, current, current + 1))
                return false;

            if (v[current] > v[current + 1])
            {
                std::swap(v[current], v[current + 1]);

                if (!visualize_step(v, renderer, current, current + 1))
                    return false;
            }
        }
    }

    return true;
}
