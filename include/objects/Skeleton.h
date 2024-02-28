#pragma once

#include <core/BufferAttribute.h>
#include <math/MathUtils.h>
#include <objects/Bone.h>
#include <textures/DataTexture.h>
#include <textures/Texture.h>

#include <vector>

namespace mix {

    class Skeleton {

    public:
        int frame{ -1 };

        std::vector<std::shared_ptr<Bone>> bones;
        std::vector<Matrix4> boneInverses;
        std::vector<float> boneMatrices;

        std::shared_ptr<DataTexture> boneTexture{ nullptr };
        int boneTextureSize{ 0 };

        [[nodiscard]] std::string uuid() const;

        void init();

        void calculateInverses();

        void pose();

        void update();

        Skeleton& computeBoneTexture();

        Bone* getBoneByName(const std::string& name);

        void dispose();

        ~Skeleton();

        static std::shared_ptr<Skeleton> create(const std::vector<std::shared_ptr<Bone>>& bones,
            const std::vector<Matrix4>& boneInverses = {});

    private:
        std::string uuid_;

        Skeleton(const std::vector<std::shared_ptr<Bone>>& bones, const std::vector<Matrix4>& boneInverses);

        friend class SkinnedMesh;
    };

}
