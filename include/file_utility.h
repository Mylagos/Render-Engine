#pragma once

#include <string_view>

namespace gpr5300
{
    std::string LoadFile(std::string_view path);
    bool TickTimer(float& tickBuffer, float dt, float tickRate);
} // namespace gpr5300
