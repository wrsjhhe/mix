#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>


namespace mix {

    struct Event {

        const std::string type;
        void* target;
    };

    struct EventListener {

        virtual void onEvent(Event& event) = 0;

        virtual ~EventListener() = default;
    };

    class EventDispatcher {

    public:
        void addEventListener(const std::string& type, EventListener* listener);

        bool hasEventListener(const std::string& type, const EventListener* listener);

        void removeEventListener(const std::string& type, const EventListener* listener);

        void dispatchEvent(const std::string& type, void* target = nullptr);

        virtual ~EventDispatcher() = default;

    private:
        std::unordered_map<std::string, std::vector<EventListener*>> listeners_;
    };

}
