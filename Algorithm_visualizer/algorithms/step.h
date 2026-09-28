#pragma once

#include <cstddef>
#include <functional>

enum class StepType { Compare, Swap, Overwrite, MarkSorted };

struct Step {
    StepType type = StepType::Compare;
    std::size_t a = 0;
    std::size_t b = 0;
    int value = 0;
};

// The algorithm modifies the vector first, then emits the event. Compare is
// emitted before comparison, Swap after a real swap, and MarkSorted means the
// index is final. A callback returning false cancels the algorithm.
using StepCallback = std::function<bool(const Step&)>;
