#include "merge_sort.h"

#include <SDL.h>
#include <vector>

#include "visualization.h"

bool merge_values(
	std::vector<int>& v,
	SDL_Renderer* renderer,
	unsigned int start,
	unsigned int middle,
	unsigned int end
)
{
	std::vector<int> merged;
	unsigned int left = start;
	unsigned int right = middle + 1;

	while (left <= middle && right <= end)
	{
		if (!visualize_step(v, renderer, left, right))
			return false;

		if (v[left] <= v[right])
		{
			merged.push_back(v[left]);
			left++;
		}
		else
		{
			merged.push_back(v[right]);
			right++;
		}
	}

	while (left <= middle)
	{
		merged.push_back(v[left]);
		left++;
	}

	while (right <= end)
	{
		merged.push_back(v[right]);
		right++;
	}

	for (unsigned int index = 0; index < merged.size(); index++)
	{
		v[start + index] = merged[index];

		if (!visualize_step(v, renderer, start + index, start + index))
			return false;
	}

	return true;
}

bool merge_values(
	std::vector<int>& v,
	SDL_Renderer* renderer,
	unsigned int start,
	unsigned int end
)
{
	if (start >= end)
		return true;

	unsigned int middle = start + (end - start) / 2;

	if (!merge_values(v, renderer, start, middle))
		return false;
	if (!merge_values(v, renderer, middle + 1, end))
		return false;

	return merge_values(v, renderer, start, middle, end);
}

bool merge_sort(std::vector<int>& v, SDL_Renderer* renderer)
{
	if (v.empty())
		return true;

	return merge_values(v, renderer, 0, v.size() - 1);
}
