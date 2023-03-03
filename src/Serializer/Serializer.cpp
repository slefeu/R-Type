#include <ECS/Components/DestroyableComponent.hpp>
#include <ECS/Components/DrawableComponent.hpp>
#include <ECS/Components/TransformComponent.hpp>
#include <Serializer/Serializer.hpp>

#include "ECS/Components/MusicComponent.hpp"
#include "NetworkLib/ISocket.hpp"

/**
 * It takes an entity, gets its position and drawable components, and serializes
 * them into a vector of bytes
 *
 * @param entity The entity to serialize
 *
 * @return A vector of uint8_t
 */

RawData Serializer::serialize(std::unique_ptr<Entity> const& entity) noexcept
{
    auto    transform   = entity->getComponent<TransformComponent>();
    auto    drawable    = entity->getComponent<DrawableComponent>();
    auto    destroyable = entity->getComponent<DestroyableComponent>();
    auto    musicable   = entity->getComponent<MusicComponent>();
    RawData data;
    auto    posX    = transform->getX();
    auto    posY    = transform->getY();
    int16_t absPosX = posX < 0 ? -posX : posX;
    int16_t absPosY = posY < 0 ? -posY : posY;

    data.reserve(15 * sizeof(std::int8_t));
    data.push_back((absPosX & 0xFF));
    data.push_back(((absPosX >> 8) & 0xFF));
    data.push_back((absPosY & 0xFF));
    data.push_back(((absPosY >> 8) & 0xFF));
    data.push_back(posX > 0 ? 1 : 0);
    data.push_back(posY > 0 ? 1 : 0);
    data.push_back(drawable->getTextureId());
    data.push_back(drawable->getWidth());
    data.push_back(drawable->getHeight());
    data.push_back(drawable->getScaleX() * 10);
    data.push_back(drawable->getScaleY() * 10);
    data.push_back(drawable->getOffsetX());
    data.push_back(drawable->getOffsetY());
    data.push_back(entity->getId());
    destroyable ? data.push_back(destroyable->getDestroyed()) : data.push_back(0);
    musicable ? data.push_back(musicable->getMusicId()) : data.push_back(0);

    return data;
}

/**
 * It takes an integer and returns a vector of bytes
 *
 * @param keyCode The key code to send.
 *
 * @return A vector of uint8_t
 */
RawData Serializer::serialize(int keyCode)
{
    RawData data;
    data.reserve(1);
    data.push_back(static_cast<uint8_t>(keyCode));
    return data;
}