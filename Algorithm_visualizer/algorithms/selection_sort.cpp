#include "selection_sort.h"

#include <algorithm>
#include <cstddef>

bool selection_sort(std::vector<int>& v, const StepCallback& on_step)
{
	const auto emit = [&](Step step)
	{
		return !on_step || on_step(step);
	};

	for (std::size_t current = 0; current < v.size(); current++)
	{
		std::size_t smallest = current;

		for (std::size_t next = current + 1; next < v.size(); next++)
		{
			if (!emit({StepType::Compare, current, next}))
				return false;

			if (v[next] < v[smallest])
				smallest = next;
		}

		if (smallest != current)
		{
			std::swap(v[current], v[smallest]);

			if (!emit({StepType::Swap, current, smallest}))
				return false;
		}

		if (!emit({StepType::MarkSorted, current}))
			return false;
	}

	return true;
}
