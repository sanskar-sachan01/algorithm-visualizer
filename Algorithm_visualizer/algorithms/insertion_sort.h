#pragma once

#include "step.h"

#include <vector>

bool insertion_sort(std::vector<int>& v, const StepCallback& on_step);
