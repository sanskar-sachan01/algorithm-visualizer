#include "quick_sort.h"

#include <SDL.h>
#include <algorithm>

#include "visualization.h"

bool quick_sort_values(
	std::vector<int>& v,
	SDL_Renderer* renderer,
	int start,
	int end
)
{
	if (start >= end)
		return true;

	int pivot = v[end];
	int smaller = start;

	for (int current = start; current < end; current++)
	{
		if (!visualize_step(v, renderer, current, end))
			return false;

		if (v[current] < pivot)
		{
			std::swap(v[current], v[smaller]);
			smaller++;

			if (!visualize_step(v, renderer, current, smaller - 1))
				return false;
		}
	}

	std::swap(v[smaller], v[end]);

	if (!visualize_step(v, renderer, smaller, end))
		return false;

	if (!quick_sort_values(v, renderer, start, smaller - 1))
		return false;

	return quick_sort_values(v, renderer, smaller + 1, end);
}

bool quick_sort(std::vector<int>& v, SDL_Renderer* renderer)
{
	if (v.empty())
		return true;

	return quick_sort_values(v, renderer, 0, v.size() - 1);
}
