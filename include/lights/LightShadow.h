#pragma once

#include <cameras/Camera.h>
#include <lights/Light.h>

#include <math/Frustum.h>
#include <math/Vector2.h>
#include <math/Vector4.h>

namespace mix {

    class RenderTarget;

    class LightShadow {

    public:
        std::shared_ptr<Camera> camera;

        float bias = 0;
        float normalBias = 0;
        float radius = 1;

        Vector2 mapSize{1024, 1024};

        std::unique_ptr<RenderTarget> map;
        std::unique_ptr<RenderTarget> mapPass;

        Matrix4 matrix;

        bool autoUpdate = true;
        bool needsUpdate = false;

        LightShadow(LightShadow&&) = delete;
        LightShadow(const LightShadow&) = delete;
        LightShadow& operator=(const LightShadow&) = delete;

        [[nodiscard]] size_t getViewportCount() const;

        [[nodiscard]] const Frustum& getFrustum() const;

        virtual void updateMatrices(Light* light, size_t viewportIndex = 0);

        Vector4& getViewport(size_t viewportIndex);

        Vector2& getFrameExtents();

        void dispose();

        virtual ~LightShadow();

    protected:
        Frustum _frustum;
        Vector2 _frameExtents{1, 1};

        Vector3 _lightPositionWorld;
        Vector3 _lookTarget;
        Matrix4 _projScreenMatrix;

        std::vector<Vector4> _viewports{Vector4(0, 0, 1, 1)};

        explicit LightShadow(std::shared_ptr<Camera> camera);
    };

}
