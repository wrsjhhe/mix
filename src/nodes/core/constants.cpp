#include <nodes/core/constants.h>

using namespace mix;

std::array<std::string, 2> mix::defaultShaderStages{ "fragment" ,"vertex" };
std::array<std::string, 3> mix::defaultBuildStages{ "setup" ,"analyze", "generate" };
std::array<std::string, 3> mix::shaderStages{ "fragment" ,"vertex","compute" };
std::array<std::string, 4> mix::vectorComponents{ "x" ,"y", "z", "w" };