#pragma once
#include <functional>

namespace mix {

    using MouseCaptureCallback = std::function<bool(void)>;
    using ScrollCaptureCallback = std::function<bool(void)>;
    using KeyboardCaptureCallback = std::function<bool(void)>;

    struct IOCapture {
        MouseCaptureCallback preventMouseEvent = [] { return false; };
        ScrollCaptureCallback preventScrollEvent = [] { return false; };
        KeyboardCaptureCallback preventKeyboardEvent = [] { return false; };
    };

}