
#pragma once

namespace mix {

    struct WindowSize {

        int width{};
        int height{};

        WindowSize() = default;

        WindowSize(int width, int height): width(width), height(height) {}

        [[nodiscard]] float aspect() const {

            return static_cast<float>(width) / static_cast<float>(height);
        }
    };

}
