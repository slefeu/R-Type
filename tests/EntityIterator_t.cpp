/*
** EPITECH PROJECT, 2023
** R-Type
** File description:
** EntityIterator_t
*/

#include <gtest/gtest.h>

#include "Entity.hpp"
#include "EntityIterator.hpp"
#include "EntityManager.hpp"
#include "HitboxComponent.hpp"
#include "TransformComponent.hpp"

TEST(EntityIterator, search_hitbox_and_position_and_check_both)
{
    std::string captured_output;

    auto manager = std::make_unique<EntityManager>();
    manager->createPlayer();
    manager->createEnemy();
    manager->createEnemy();
    manager->createPlayer();
    manager->createEnemy();
    manager->createPlayer();

    auto it = EntityIterator<HitboxComponent, TransformComponent>(manager->getEntities());

    testing::internal::CaptureStdout();

    while (!it.isEnd()) {
        std::cout << it.get()->getId() << std::endl;
        ++it;
    }
    it.reset();

    while (!it.isEnd()) {
        std::cout << it.get()->getId() << std::endl;
        ++it;
    }
    it.reset();

    captured_output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(captured_output, "0\n3\n5\n0\n3\n5\n");
}

TEST(EntityIterator, search_position_and_check_for_hitbox)
{
    auto manager = std::make_unique<EntityManager>();
    manager->createEnemy();
    manager->createEnemy();
    manager->createEnemy();

    auto it = EntityIterator<TransformComponent>(manager->getEntities());

    while (!it.isEnd()) {
        EXPECT_FALSE((it.get()->hasComponents<HitboxComponent>()));
        ++it;
    }
    it.reset();
}