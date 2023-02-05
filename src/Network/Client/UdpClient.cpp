/*
** EPITECH PROJECT, 2023
** R-Type
** File description:
** UdpClient
*/

#include "UdpClient.hpp"

#include <iostream>
#include <memory>

#include "Deserializer.hpp"
#include "DrawableComponent.hpp"
#include "Error.hpp"
#include "SocketFactory.hpp"
#include "SocketSelectorFactory.hpp"

[[nodiscard]] UdpClient::UdpClient(Address serverAddress, Address::Port clientPort)
    : serverAddress_(serverAddress)
    , socket_(SocketFactory::createSocket(clientPort))
    , selector_(SocketSelectorFactory::createSocketSelector())
    , deserializer_(std::make_unique<Deserializer>())
{
    selector_->add(*socket_, true, true, false);

    gameThread_    = std::thread([&]() { gameLoop(); });
    networkThread_ = std::thread([&]() { communicate(); });

    std::cout << "UdpClient created" << std::endl;
}

void UdpClient::stop() noexcept
{
    looping_ = false;
}

void UdpClient::reset() noexcept
{
    looping_ = true;
}

void UdpClient::run()
{
    gameThread_.join();
    networkThread_.join();
}

void UdpClient::communicate() noexcept
{
    // TODO : send a connection request to the server
    RawData data = { 12 };
    dataToSend_.push(data);

    while (looping_) {
        try {
            selector_->select(true, true, false);
        } catch (const NetworkExecError& message) {
            std::cerr << message.what() << std::endl;
        }
        if (selector_->isSet(*socket_, SocketSelector::Operation::READ)) { receive(); }
        if (selector_->isSet(*socket_, SocketSelector::Operation::WRITE)) { send(); }
    }
}

void UdpClient::gameLoop() noexcept
{
    auto& lib = game_.getLib();
    lib->getWindow().open(256, 256, "Client RTYPE");

    while (looping_) {
        auto input = lib->getWindow().getKeyPressed();
        if (input == 255 || input == 36) {
            looping_ = false;
            break;
        }
        if (input != 0) { dataToSend_.push({ (uint8_t)input }); }
        game_.run();
    }

    dataToSend_.push({ CLOSE_VALUE });
    lib->getWindow().close();
}

void UdpClient::receive()
{
    try {
        ReceivedInfos infoReceived = socket_->receive();
        handleData(infoReceived);
    } catch (const NetworkExecError& e) {
        std::cerr << e.what() << std::endl;
    }
}

void UdpClient::send()
{
    if (dataToSend_.size() != 0) {
        auto blob = getDataFromQueue();
        try {
            socket_->send(blob.data(), blob.size(), serverAddress_);
        } catch (const NetworkExecError& e) {
            std::cerr << e.what() << std::endl;
            dataToSend_.push(blob);
        }
    }
}

RawData UdpClient::getDataFromQueue() noexcept
{
    RawData blob = dataToSend_.front();
    dataToSend_.pop();
    return (blob);
}

void UdpClient::handleData(ReceivedInfos infos) noexcept
{
    if (infos.data.size() < 2) return;

    if (infos.data.size() % 12 == 0) {
        for (int i = 0; i < infos.data.size(); i += 12) {
            deserializeEntity(infos.data[i],
                infos.data[i + 1],
                infos.data[i + 2],
                infos.data[i + 3],
                infos.data[i + 4],
                infos.data[i + 5],
                infos.data[i + 6],
                infos.data[i + 7],
                infos.data[i + 8],
                infos.data[i + 9],
                infos.data[i + 10],
                infos.data[i + 11]);
        }
    }
    infos.data.clear();
}

void UdpClient::deserializeEntity(int x,
    int                               xpositive,
    int                               y,
    int                               ypositive,
    int                               idSprite,
    int                               width,
    int                               height,
    int                               scaleX,
    int                               scaleY,
    int                               offsetX,
    int                               offsetY,
    int                               id) noexcept
{
    auto& manager  = game_.getManager();
    auto  m_entity = manager->getEntity(id);

    if (m_entity == nullptr) {
        std::unique_ptr<Entity> entity = std::make_unique<Entity>(manager->createId());

        auto transform = TransformComponent(xpositive ? x : -x, ypositive ? y : -y);
        transform.setScale(scaleX, scaleY);

        auto drawable = DrawableComponent(offsetX, offsetY, width, height, idSprite);
        game_.addSprite("assets/r-typesheet" + std::to_string(idSprite) + ".gif", x, y);
        drawable.setSprite(game_.getLastSprite());

        entity->addComponent(transform);
        entity->addComponent(drawable);

        manager->addEntity(entity);
    } else {
        auto transform = m_entity->getComponent<TransformComponent>();
        auto drawable  = m_entity->getComponent<DrawableComponent>();

        std::cout << m_entity->getId() << " | x : " << x << " y : " << y;
        transform->setX(xpositive ? x : -x);
        transform->setY(ypositive ? y : -y);
        std::cout << " | x : " << transform->getX() << " y : " << transform->getY() << std::endl;
        transform->setScale((float)scaleX / 10, (float)scaleY / 10);
        drawable->setOffsetX(offsetX);
        drawable->setOffsetY(offsetY);
        drawable->setWidth(width);
        drawable->setHeight(height);
        // drawable->setTextureId(idSprite);
    }
}
