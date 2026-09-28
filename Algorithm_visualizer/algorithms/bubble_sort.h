#pragma once

#include "step.h"

#include <vector>

bool bubble_sort(std::vector<int>& v, const StepCallback& on_step);
