#include "insertion_sort.h"

#include <algorithm>
#include <cstddef>

bool insertion_sort(std::vector<int>& v, const StepCallback& on_step)
{
	const auto emit = [&](Step step)
	{
		return !on_step || on_step(step);
	};

	for (std::size_t current = 1; current < v.size(); current++)
	{
		std::size_t position = current;

		while (position > 0)
		{
			if (!emit({StepType::Compare, position - 1, position}))
				return false;

			if (v[position - 1] <= v[position])
				break;

			const std::size_t left = position - 1;
			std::swap(v[left], v[position]);
			if (!emit({StepType::Swap, left, position}))
				return false;

			position--;
		}
	}

	for (std::size_t index = 0; index < v.size(); index++)
	{
		if (!emit({StepType::MarkSorted, index}))
			return false;
	}

	return true;
}
