#include <nodes/core/constants.h>

using namespace mix;

std::array<std::string, 2> defaultShaderStages{ "fragment" ,"vertex" };
std::array<std::string, 3> defaultBuildStages{ "setup" ,"analyze", "generate" };
std::array<std::string, 3> shaderStages{ "fragment" ,"vertex","compute" };
std::array<std::string, 4> vectorComponents{ "x" ,"y", "z", "w" };