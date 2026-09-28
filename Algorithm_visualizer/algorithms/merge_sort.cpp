#include "merge_sort.h"

#include <cstddef>
#include <vector>

namespace
{
bool emit_step(const StepCallback& on_step, Step step)
{
	return !on_step || on_step(step);
}

bool merge_runs(
	std::vector<int>& v,
	const StepCallback& on_step,
	std::size_t start,
	std::size_t middle,
	std::size_t end,
	bool mark_sorted
)
{
	std::vector<int> merged;
	std::size_t left = start;
	std::size_t right = middle + 1;

	while (left <= middle && right <= end)
	{
		if (!emit_step(on_step, {StepType::Compare, left, right}))
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

	for (std::size_t index = 0; index < merged.size(); index++)
	{
		const std::size_t position = start + index;
		v[position] = merged[index];

		if (!emit_step(
				on_step,
				{StepType::Overwrite, position, position, merged[index]}))
			return false;

		if (mark_sorted && !emit_step(on_step, {StepType::MarkSorted, position}))
			return false;
	}

	return true;
}

bool merge_sort_range(
	std::vector<int>& v,
	const StepCallback& on_step,
	std::size_t start,
	std::size_t end,
	bool mark_sorted
)
{
	if (start >= end)
		return true;

	const std::size_t middle = start + (end - start) / 2;

	if (!merge_sort_range(v, on_step, start, middle, false))
		return false;
	if (!merge_sort_range(v, on_step, middle + 1, end, false))
		return false;

	return merge_runs(v, on_step, start, middle, end, mark_sorted);
}
}

bool merge_sort(std::vector<int>& v, const StepCallback& on_step)
{
	if (v.empty())
		return true;

	if (v.size() == 1)
		return emit_step(on_step, {StepType::MarkSorted, 0});

	return merge_sort_range(v, on_step, 0, v.size() - 1, true);
}
