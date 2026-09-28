#include "insertion_sort.h"

#include <SDL.h>

#include "visualization.h"

bool insertion_sort(std::vector<int>& v, SDL_Renderer* renderer)
{
	for (unsigned int current = 1; current < v.size(); current++)
	{
		unsigned int position = current;

		while (position > 0)
		{
			if (!visualize_step(v, renderer, position - 1, position))
				return false;

			if (v[position - 1] <= v[position])
				break;

			std::swap(v[position - 1], v[position]);
			position--;

			if (!visualize_step(v, renderer, position, position + 1))
				return false;
		}
	}

	return true;
}
