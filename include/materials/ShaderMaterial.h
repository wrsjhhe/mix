#pragma once

#include <materials/interfaces.h>
#include <materials/Material.h>

namespace mix {
    class ShaderMaterial : public virtual Material,
        public MaterialWithClipping,
        public MaterialWithLights,
        public MaterialWithWireframe,
        public MaterialWithLineWidth,
        public MaterialWithDefines {

    public:
        std::string vertexShader;
        std::string fragmentShader;
        std::shared_ptr<UniformMap> uniforms;

        std::optional<std::string> index0AttributeName;
        bool uniformsNeedUpdate = false;

        [[nodiscard]] std::string type() const override;

        static std::shared_ptr<ShaderMaterial> create();

    protected:
        ShaderMaterial();
    };

}