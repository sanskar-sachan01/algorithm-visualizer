#include "selection_sort.h"

#include <SDL.h>
#include <algorithm>

#include "visualization.h"

bool selection_sort(std::vector<int>& v, SDL_Renderer* renderer)
{
	for (unsigned int current = 0; current < v.size(); current++)
	{
		unsigned int smallest = current;

		for (unsigned int next = current + 1; next < v.size(); next++)
		{
			if (!visualize_step(v, renderer, smallest, next))
				return false;

			if (v[next] < v[smallest])
				smallest = next;
		}

		if (smallest != current)
		{
			std::swap(v[current], v[smallest]);

			if (!visualize_step(v, renderer, current, smallest))
				return false;
		}
	}

	return true;
}
