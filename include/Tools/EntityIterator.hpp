/*
** EPITECH PROJECT, 2023
** R-Type
** File description:
** Iterator
*/

#pragma once

#include <vector>

#include "Entity.hpp"

/**
 * @brief A template class that is used to iterate over entities that have a certain set of components.
 *
 * @tparam Args The components that the entities must have
 */
template <typename... Args>
class EntityIterator
{
  public:
    EntityIterator(typename std::vector<std::unique_ptr<Entity>>& entities)
        : it(entities)
    {
        skip();
    }

    EntityIterator& operator++()
    {
        ++idx;
        skip();
        return *this;
    };

    std::unique_ptr<Entity>& get() { return it[idx]; }

    int                                   idx = 0;
    std::vector<std::unique_ptr<Entity>>& it;

    bool isEnd() { return idx >= it.size(); }

    void reset() { idx = 0; }

  private:
    void skip()
    {
        while (idx < it.size() && !it[idx]->hasComponents<Args...>()) { ++idx; }
    }
};