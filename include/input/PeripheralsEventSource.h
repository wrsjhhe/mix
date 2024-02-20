#pragma once

#include <input/IOCapture.h>
#include <input/KeyListener.h>
#include <input/MouseListener.h>

#include <canvas/WindowSize.h>

#include <vector>

namespace mix {

    class PeripheralsEventSource {

    public:
        [[nodiscard]] virtual WindowSize size() const = 0;

        void setIOCapture(IOCapture* capture);

        void addKeyListener(KeyListener& listener);

        bool removeKeyListener(const KeyListener& listener);

        void addMouseListener(MouseListener& listener);

        bool removeMouseListener(const MouseListener& listener);

        virtual ~PeripheralsEventSource() = default;

    protected:
        enum class KeyAction {
            PRESS,
            RELEASE,
            REPEAT
        };

        enum class MouseAction {
            PRESS,
            RELEASE
        };

        void onMousePressedEvent(int button, const Vector2& pos, MouseAction action);

        void onMouseMoveEvent(const Vector2& pos);

        void onMouseWheelEvent(const Vector2& eventData);

        void onKeyEvent(KeyEvent evt, KeyAction action);

    private:
        IOCapture* ioCapture_ = nullptr;
        std::vector<KeyListener*> keyListeners_;
        std::vector<MouseListener*> mouseListeners_;
    };

}
