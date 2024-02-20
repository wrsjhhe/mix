
#pragma once

#include <math/Vector2.h>

#include <functional>
#include <utility>

namespace mix {

    struct MouseEvent {

        const int button;
        const int action;
        const int mods;

        MouseEvent(const int button, const int action, const int mods)
            : button(button), action(action), mods(mods) {}
    };

    struct MouseListener {

        virtual void onMouseDown(int button, const Vector2& pos) {}
        virtual void onMouseUp(int button, const Vector2& pos) {}
        virtual void onMouseMove(const Vector2& pos) {}
        virtual void onMouseWheel(const Vector2& delta) {}

        virtual ~MouseListener() = default;
    };

    struct MouseMoveListener: MouseListener {

        explicit MouseMoveListener(std::function<void(const Vector2&)> f)
            : f_(std::move(f)) {}

        void onMouseMove(const Vector2& pos) override {
            f_(pos);
        }

    private:
        std::function<void(Vector2)> f_;
    };

}
