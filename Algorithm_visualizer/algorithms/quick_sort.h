#pragma once

#include "step.h"

#include <vector>

bool quick_sort(std::vector<int>& v, const StepCallback& on_step);
