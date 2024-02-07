#pragma once

namespace mix {

    class Layers {

    public:
        Layers();

        void set(unsigned int channel);

        void enable(unsigned int channel);

        void enableAll();

        void toggle(unsigned int channel);

        void disable(unsigned int channel);

        void disableAll();

        [[nodiscard]] bool test(const Layers& layers) const;

        [[nodiscard]] bool isEnabled(unsigned int channel) const;

        [[nodiscard]] unsigned int mask() const;

    private:
        unsigned int mask_;

        friend class Object3D;
    };

}
