#pragma once

#include "fmt/format.h"

#define LOG(...) std::cout << fmt::format(__VA_ARGS__) << '\n';
