#pragma once

#include <core/Uniform.h>

#include <string>

namespace mix {

    struct Shader {

        UniformMap uniforms;
        std::string vertexShader;
        std::string fragmentShader;
    };

}
