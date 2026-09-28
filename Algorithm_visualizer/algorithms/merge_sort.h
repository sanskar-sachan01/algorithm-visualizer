#pragma once

#include "step.h"

#include <vector>

bool merge_sort(std::vector<int>& v, const StepCallback& on_step);
