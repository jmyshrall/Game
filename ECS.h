#pragma once
#include <unordered_map>
#include <memory>
#include <typeindex>
#include <vector>
#include <cstdint>

using EntityID = uint32_t;

// base component type
struct Component { virtual ~Component() = default; };

class ECS
{
public:
    static ECS& get();

    EntityID createEntity() { return m_nextID++; }
    void destroyEntity(EntityID id) { m_store.erase(id); }

    template<typename T, typename... Args>
    T& addComponent(EntityID id, Args&&... args) {
        auto comp = std::make_unique<T>(std::forward<Args>(args)...);
        T& ref = *comp;
        m_store[id][typeid(T)] = std::move(comp);
        return ref;
    }

    template<typename T>
    T* getComponent(EntityID id) {
        auto eit = m_store.find(id);
        if (eit == m_store.end()) return nullptr;
        auto cit = eit->second.find(typeid(T));
        if (cit == eit->second.end()) return nullptr;
        return static_cast<T*>(cit->second.get());
    }

    template<typename T>
    bool hasComponent(EntityID id) { return getComponent<T>(id) != nullptr; }

    const std::unordered_map<EntityID,
          std::unordered_map<std::type_index, std::unique_ptr<Component>>>& entities() const {
        return m_store;
    }

private:
    ECS() = default;
    EntityID m_nextID = 1;
    std::unordered_map<EntityID,
        std::unordered_map<std::type_index, std::unique_ptr<Component>>> m_store;
};
