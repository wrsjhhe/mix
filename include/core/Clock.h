#pragma once

#include <memory>

namespace mix {

    class Clock {

    public:
        bool autoStart;
        bool running = false;

        float elapsedTime = 0;

        explicit Clock(bool autoStart = true);

        void start();

        void stop();

        float getElapsedTime();

        float getDelta();

        ~Clock();

    private:
        struct Impl;
        std::unique_ptr<Impl> pimpl_;
    };

}
