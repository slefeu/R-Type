/*
** EPITECH PROJECT, 2023
** R-Type
** File description:
** HitboxComponent Unit Test
*/

#include <gtest/gtest.h>

#include "Entity.hpp"
#include "HitboxComponent.hpp"

TEST(HitboxComponent_, getWidth)
{
    auto hitbox = std::make_unique<HitboxComponent>(10, 10);
    EXPECT_EQ(hitbox->getWidth(), 10);
}

TEST(HitboxComponent_, getHeight)
{
    auto hitbox = std::make_unique<HitboxComponent>(10, 10);
    EXPECT_EQ(hitbox->getHeight(), 10);
}

TEST(HitboxComponent_, setWidth)
{
    auto hitbox = std::make_unique<HitboxComponent>(10, 10);
    hitbox->setWidth(20);
    EXPECT_EQ(hitbox->getWidth(), 20);
}

TEST(HitboxComponent_, setHeight)
{
    auto hitbox = std::make_unique<HitboxComponent>(10, 10);
    hitbox->setHeight(20);
    EXPECT_EQ(hitbox->getHeight(), 20);
}

TEST(HitboxComponent_, setSize)
{
    auto hitbox = std::make_unique<HitboxComponent>(10, 10);
    hitbox->setSize(20, 20);
    EXPECT_EQ(hitbox->getWidth(), 20);
    EXPECT_EQ(hitbox->getHeight(), 20);
}