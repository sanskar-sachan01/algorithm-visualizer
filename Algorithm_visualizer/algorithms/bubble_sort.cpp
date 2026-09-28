#include "bubble_sort.h"

#include <algorithm>
#include <cstddef>

bool bubble_sort(std::vector<int>& v, const StepCallback& on_step)
{
    const auto emit = [&](Step step)
    {
        return !on_step || on_step(step);
    };

    for (std::size_t end = v.size(); end > 1; end--)
    {
        bool swapped = false;

        for (std::size_t current = 0; current + 1 < end; current++)
        {
            if (!emit({StepType::Compare, current, current + 1}))
                return false;

            if (v[current] > v[current + 1])
            {
                std::swap(v[current], v[current + 1]);
                swapped = true;

                if (!emit({StepType::Swap, current, current + 1}))
                    return false;
            }
        }

        if (!emit({StepType::MarkSorted, end - 1}))
            return false;

        if (!swapped)
        {
            for (std::size_t index = 0; index + 1 < end; index++)
            {
                if (!emit({StepType::MarkSorted, index}))
                    return false;
            }

            return true;
        }
    }

    if (!v.empty() && !emit({StepType::MarkSorted, 0}))
        return false;

    return true;
}
