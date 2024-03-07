#pragma once

#include <unordered_map>
#include <memory>

namespace mix {
    class WeakMap {
    public:
        void* get(void* key) {
            auto iter = map.find(key);
            if (iter == map.end()) {
                return nullptr;
            }

            auto weakPtr = iter->second;
            if (weakPtr.expired()) {
                map.erase(iter);
                return nullptr;
            }
            return iter->second.lock().get();
        }

        void set(void* key, const std::shared_ptr<void>& value) {
            map[key] = value;
        }

        void remove(void* key) {
            auto iter = map.find(key);
            if (iter != map.end()) {
                map.erase(iter);
            }
        }

        size_t size() {
            auto iter = map.begin();
            while (iter != map.end())
            {
                if (iter->second.expired()) {
                    iter = map.erase(iter);
                }
                ++iter;
            }
            return map.size();
        }

        bool has(void* key) {
            return get(key) != nullptr;
        }

        void dispose() {
            map.clear();
        }

    private:
        std::unordered_map<void*, std::weak_ptr<void>> map;
    };

    typedef WeakMap DataMap;
}