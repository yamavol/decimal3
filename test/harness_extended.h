#pragma once
/**
 * This module defines additional test assertion apis, unique to this project.
 */
#include "harness.h"
#include <functional>

void ASSERT_THROW(test_runner* runner, std::function<void()> function, const char* msg, ...);

void ASSERT_NO_THROW(test_runner* runner, std::function<void()> function, const char* msg, ...);
