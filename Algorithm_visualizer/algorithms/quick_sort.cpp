#include "quick_sort.h"

#include <algorithm>
#include <cstddef>

namespace
{
bool emit_step(const StepCallback& on_step, Step step)
{
	return !on_step || on_step(step);
}

bool quick_sort_range(
	std::vector<int>& v,
	const StepCallback& on_step,
	int start,
	int end
)
{
	if (start > end)
		return true;

	if (start == end)
		return emit_step(
			on_step,
			{StepType::MarkSorted, static_cast<std::size_t>(start)}
		);

	const int pivot = v[static_cast<std::size_t>(end)];
	int smaller = start;

	for (int current = start; current < end; current++)
	{
		if (!emit_step(
				on_step,
				{
					StepType::Compare,
					static_cast<std::size_t>(current),
					static_cast<std::size_t>(end)
				}))
			return false;

		if (v[static_cast<std::size_t>(current)] < pivot)
		{
			std::swap(
				v[static_cast<std::size_t>(current)],
				v[static_cast<std::size_t>(smaller)]
			);

			if (smaller != current && !emit_step(
					on_step,
					{
						StepType::Swap,
						static_cast<std::size_t>(smaller),
						static_cast<std::size_t>(current)
					}))
				return false;

			smaller++;
		}
	}

	std::swap(
		v[static_cast<std::size_t>(smaller)],
		v[static_cast<std::size_t>(end)]
	);

	if (smaller != end && !emit_step(
			on_step,
			{
				StepType::Swap,
				static_cast<std::size_t>(smaller),
				static_cast<std::size_t>(end)
			}))
		return false;

	if (!emit_step(
			on_step,
			{StepType::MarkSorted, static_cast<std::size_t>(smaller)}))
		return false;

	if (!quick_sort_range(v, on_step, start, smaller - 1))
		return false;

	return quick_sort_range(v, on_step, smaller + 1, end);
}
}

bool quick_sort(std::vector<int>& v, const StepCallback& on_step)
{
	if (v.empty())
		return true;

	return quick_sort_range(v, on_step, 0, static_cast<int>(v.size() - 1));
}
