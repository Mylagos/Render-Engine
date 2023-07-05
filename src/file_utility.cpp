#include "file_utility.h"
#include <fstream>

namespace gpr5300
{
std::string LoadFile(std::string_view path)
{
    std::string content;
    std::ifstream t(path.data());

    t.seekg(0, std::ios::end);
    content.reserve(t.tellg());
    t.seekg(0, std::ios::beg);

    content.assign((std::istreambuf_iterator<char>(t)),
        std::istreambuf_iterator<char>());
    return content;
}

bool TickTimer(float& tickBuffer, const float dt, const float tickRate)
{
    tickBuffer += dt;
    if (tickBuffer>= tickRate)
    {
        return true;
    }
    return false;
}
} // namespace gpr5300
