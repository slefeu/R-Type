/*
** EPITECH PROJECT, 2022
** YEP
** File description:
** HitboxSystem.cpp
*/

#include <ECS/Systems/DrawableSystem.hpp>
#include <cassert>
#include <iostream>
#include <memory>

/**
 * It takes a reference to a unique pointer to an EntityManager, and then it initializes the manager_
 * member variable with that reference, and it initializes the it_ member variable with an
 * EntityIterator that iterates over the entities in the EntityManager
 *
 * @param EntityManager The EntityManager that the system will use to iterate over entities.
 */
DrawableSystem::DrawableSystem(EntityManager* manager) noexcept
    : manager_(manager)
    , it_(manager->getEntities())
{
}

/**
 * It iterates through all entities with a BehaviorComponent and calls their onUpdate() function
 */
void DrawableSystem::run()
{
    if (!window_) { return; }

    window_->clear();

    for (; !it_.isEnd(); ++it_) {
        Entity* entity = it_.get();

        assert((entity->hasComponents<DrawableComponent, TransformComponent>()));

        auto transform = entity->getComponent<TransformComponent>();
        auto drawable  = entity->getComponent<DrawableComponent>();

        drawable->getSprite().setX(transform->getX());
        drawable->getSprite().setY(transform->getY());
        window_->draw(drawable->getSprite(),
            drawable->getOffsetX(),
            drawable->getOffsetY(),
            drawable->getWidth(),
            drawable->getHeight());
    }
    it_.reset();

    window_->refresh();
}

void DrawableSystem::setWindow(Window* window) noexcept
{
    window_ = window;
}