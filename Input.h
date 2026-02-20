#pragma once
#include <unordered_map>

enum class Key
{
    W, A, S, D,
    Up, Down, Left, Right,
    Space, Escape,
    Unknown
};

class Input
{
public:
    static Input& get();

    void pressKey(Key k)   { m_keys[k] = true; }
    void releaseKey(Key k) { m_keys[k] = false; }

    bool isKeyDown(Key k) const {
        auto it = m_keys.find(k);
        return it != m_keys.end() && it->second;
    }

private:
    Input() = default;
    std::unordered_map<Key, bool> m_keys;
};
