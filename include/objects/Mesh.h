#pragma once

#include <core/BufferGeometry.h>
#include <core/Object3D.h>
#include <materials/Material.h>
#include <objects/ObjectWithMorphTargetInfluences.h>


namespace mix {

    // Class representing triangular polygon mesh based objects.
    class Mesh : public Object3D, public ObjectWithMorphTargetInfluences {

    public:

        explicit Mesh(std::shared_ptr<BufferGeometry> geometry = nullptr, std::shared_ptr<Material> material = nullptr);
        Mesh(std::shared_ptr<BufferGeometry> geometry, std::vector<std::shared_ptr<Material>> materials);

        Mesh(Mesh&& other) noexcept;

        [[nodiscard]] std::string type() const override;

        BufferGeometry* geometry() override;

        std::shared_ptr<BufferGeometry> shared_geometry() {

            return geometry_;
        }

        [[nodiscard]] const BufferGeometry* geometry() const;

        void setGeometry(const std::shared_ptr<BufferGeometry>& geometry);

        Material* material() override;

        [[nodiscard]] std::vector<Material*> materials() override;

        void setMaterial(const std::shared_ptr<Material>& material);

        void setMaterials(const std::vector<std::shared_ptr<Material>>& materials);

        [[nodiscard]] size_t numMaterials() const;

        void raycast(Raycaster& raycaster, std::vector<Intersection>& intersects) override;

        std::shared_ptr<Object3D> clone(bool recursive = true) override;

        static std::shared_ptr<Mesh> create(
            std::shared_ptr<BufferGeometry> geometry = nullptr,
            std::shared_ptr<Material> material = nullptr);

        static std::shared_ptr<Mesh> create(
            std::shared_ptr<BufferGeometry> geometry,
            std::vector<std::shared_ptr<Material>> materials);

        ~Mesh() override = default;

    protected:
        std::shared_ptr<BufferGeometry> geometry_;
        std::vector<std::shared_ptr<Material>> materials_;
    };

}