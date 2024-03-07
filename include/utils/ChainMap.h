#pragma once 

#include <vector>
#include <memory>
#include <unordered_map>

namespace mix {

    class ChainMap {
    private:
        struct Node {
            std::weak_ptr<void> value;
            std::unordered_map<void*, std::shared_ptr<Node>> children;
        };
    public:
        void* get(const std::vector<void*>& keys) {
            std::unordered_map<void*, std::shared_ptr<Node>>* children = &root->children;
            for (size_t i = 0; i < keys.size(); ++i) {
                void* key = keys[i];
                auto iter = children->find(key);
                if (children->find(key) == children->end()) {
                    return nullptr;
                }
                children = &iter->second->children;
            }
            auto iter = children->find(keys[keys.size() - 1]);
            if (iter == children->end()) {
                return nullptr;
            }
            auto weakPtr = iter->second->value;
            if (weakPtr.expired()) {
                remove(keys);
                return nullptr;
            }

            return iter->second->value.lock().get();
        }

        void* get(void* key) {
            auto& children = root->children;
            auto iter = children.find(key);
            if (iter == children.end()) {
                return nullptr;
            }
            auto weakPtr = iter->second->value;
            if (weakPtr.expired()) {
                remove(key);
                return nullptr;
            }
            return iter->second->value.lock().get();
        }

        void set(const std::vector<void*>& keys, std::shared_ptr<void> value) {
            std::unordered_map<void*, std::shared_ptr<Node>>* children = &root->children;
            for (size_t i = 0; i < keys.size(); ++i) {
                void* key = keys[i];
                auto iter = children->find(key);
                if (children->find(key) == children->end()) {
                    children->insert({ key , std::make_shared<Node>() });
                    iter = children->find(key);
                }
                children = &iter->second->children;
            }

            std::pair<void*, std::shared_ptr<Node>> newNode{ keys[keys.size() - 1] ,std::make_shared<Node>() };
            newNode.second->value = value;
            children->insert(newNode);
        }

        void set(void* key, std::shared_ptr<void> value) {
            auto& children = root->children;
            children[key] = std::make_shared<Node>();
            children[key]->value = value;
        }

        void remove(const std::vector<void*>& keys) {
            std::unordered_map<void*, std::shared_ptr<Node>>* children = &root->children;
            for (size_t i = 0; i < keys.size(); ++i) {
                void* key = keys[i];
                auto iter = children->find(key);
                if (iter == children->end()) {
                    return;
                }
                children = &iter->second->children;
            }
            auto iter = children->find(keys[keys.size() - 1]);
            if (iter == children->end()) {
                return;
            }
            children->erase(iter);
        }

        void remove(void* key) {
            auto& children = root->children;
            auto iter = children.find(key);
            if (iter == children.end()) {
                return;
            }
            children.erase(iter);
        }


    private:
        std::shared_ptr<Node> root = std::make_shared<Node>();
    };

};