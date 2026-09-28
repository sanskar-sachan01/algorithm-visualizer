#include "algorithms/bubble_sort.h"
#include "algorithms/insertion_sort.h"
#include "algorithms/merge_sort.h"
#include "algorithms/quick_sort.h"
#include "algorithms/selection_sort.h"

#include <algorithm>
#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace
{
using SortFunction = bool (*)(std::vector<int>&, const StepCallback&);

void expect(bool condition, const std::string& message)
{
    if (!condition)
        throw std::runtime_error(message);
}

struct EventSummary
{
    std::vector<Step> events;
    std::vector<int> shadow;
    std::vector<int> marks;
    std::size_t comparisons = 0;
    std::size_t swaps = 0;
    std::size_t overwrites = 0;
};

EventSummary run_sort(
    SortFunction sort,
    const std::vector<int>& input,
    const std::vector<int>& expected,
    const std::string& name
)
{
    std::vector<int> values = input;
    EventSummary summary;
    summary.shadow = input;
    summary.marks.assign(input.size(), 0);

    const bool completed = sort(values, [&](const Step& step)
    {
        summary.events.push_back(step);

        if (step.type == StepType::Compare)
        {
            expect(step.a < values.size() && step.b < values.size(),
                   name + ": Compare index out of range");
            expect(values == summary.shadow,
                   name + ": Compare changed the array before comparison");
            summary.comparisons++;
        }
        else if (step.type == StepType::Swap)
        {
            expect(step.a < values.size() && step.b < values.size(),
                   name + ": Swap index out of range");
            expect(step.a != step.b,
                   name + ": Swap used identical indices");

            std::swap(summary.shadow[step.a], summary.shadow[step.b]);
            expect(values == summary.shadow,
                   name + ": Swap does not describe the actual mutation");
            summary.swaps++;
        }
        else if (step.type == StepType::Overwrite)
        {
            expect(step.a < values.size() && step.b < values.size(),
                   name + ": Overwrite index out of range");
            expect(step.a == step.b,
                   name + ": Overwrite indices differ");

            summary.shadow[step.a] = step.value;
            expect(values == summary.shadow,
                   name + ": Overwrite does not describe the actual mutation");
            summary.overwrites++;
        }
        else if (step.type == StepType::MarkSorted)
        {
            expect(step.a < values.size(),
                   name + ": MarkSorted index out of range");
            expect(values == summary.shadow,
                   name + ": MarkSorted changed the array");
            summary.marks[step.a]++;
        }

        return true;
    });

    expect(completed, name + ": algorithm was cancelled unexpectedly");
    expect(values == expected, name + ": final array is not sorted correctly");
    expect(summary.shadow == values, name + ": event stream replay diverged");

    if (input.empty())
    {
        expect(summary.events.empty(), name + ": empty input emitted events");
    }
    else
    {
        for (std::size_t index = 0; index < input.size(); index++)
            expect(summary.marks[index] == 1,
                   name + ": MarkSorted coverage is not exactly once");
    }

    if (input.size() > 1)
        expect(summary.comparisons > 0,
               name + ": no Compare events were emitted");

    return summary;
}

void test_all_input_shapes(SortFunction sort, const std::string& name)
{
    const std::vector<std::vector<int>> inputs{
        {5, 1, 4, 2, 3},
        {1, 2, 3, 4, 5},
        {5, 4, 3, 2, 1},
        {4, 2, 4, 1, 2, 4},
        {7},
        {}
    };

    for (const std::vector<int>& input : inputs)
    {
        std::vector<int> expected = input;
        std::sort(expected.begin(), expected.end());
        run_sort(sort, input, expected, name);
    }
}

void test_selection_keeps_full_comparison_behavior()
{
    const std::vector<int> input{1, 2, 3, 4, 5};
    const EventSummary summary = run_sort(
        selection_sort,
        input,
        input,
        "Selection Sort full comparisons"
    );

    expect(summary.comparisons == 10,
           "Selection Sort changed its normal comparison behavior");
}

void test_merge_emits_write_back_events()
{
    const std::vector<int> input{4, 1, 3, 2};
    const EventSummary summary = run_sort(
        merge_sort,
        input,
        {1, 2, 3, 4},
        "Merge Sort write-back"
    );

    expect(summary.overwrites >= input.size(),
           "Merge Sort emitted too few Overwrite events");
}

void test_quick_first_lomuto_partition()
{
    const std::vector<int> input{3, 1, 2};
    const EventSummary summary = run_sort(
        quick_sort,
        input,
        {1, 2, 3},
        "Quick Sort Lomuto partition"
    );

    expect(summary.events.size() >= 5,
           "Quick Sort emitted too few partition events");
    expect(summary.events[0].type == StepType::Compare &&
               summary.events[0].a == 0 && summary.events[0].b == 2,
           "Quick Sort did not compare against the last-element pivot");
    expect(summary.events[1].type == StepType::Compare &&
               summary.events[1].a == 1 && summary.events[1].b == 2,
           "Quick Sort changed its Lomuto comparison order");
    expect(summary.events[2].type == StepType::Swap &&
               summary.events[2].a == 0 && summary.events[2].b == 1,
           "Quick Sort did not emit the first partition swap");
    expect(summary.events[3].type == StepType::Swap &&
               summary.events[3].a == 1 && summary.events[3].b == 2,
           "Quick Sort did not emit the pivot placement swap");
    expect(summary.events[4].type == StepType::MarkSorted &&
               summary.events[4].a == 1,
           "Quick Sort did not mark the pivot's final position");
}
}

int main()
{
    try
    {
        test_all_input_shapes(bubble_sort, "Bubble Sort");
        test_all_input_shapes(insertion_sort, "Insertion Sort");
        test_all_input_shapes(selection_sort, "Selection Sort");
        test_all_input_shapes(merge_sort, "Merge Sort");
        test_all_input_shapes(quick_sort, "Quick Sort");
        test_selection_keeps_full_comparison_behavior();
        test_merge_emits_write_back_events();
        test_quick_first_lomuto_partition();

        std::cout << "All algorithm tests passed.\n";
        return 0;
    }
    catch (const std::exception& error)
    {
        std::cerr << "Test failure: " << error.what() << '\n';
        return 1;
    }
}
