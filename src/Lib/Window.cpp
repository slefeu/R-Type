/*
** EPITECH PROJECT, 2022
** YEP
** File description:
** Window.cpp
*/

#include <Lib/Window.hpp>
#include <iostream>

Window::Window() noexcept {}

/**
 * It opens the window
 */
void Window::open(int width, int height, const std::string& title)
{
    window.create(sf::VideoMode(width, height), title);
    window.setFramerateLimit(30);
}

/**
 * It closes the window
 */
void Window::close()
{
    window.close();
}

/**
 * It clears the window
 */
void Window::clear()
{
    window.clear();
}

/**
 * It refreshes the window
 */
void Window::refresh()
{
    window.display();
}

/**
 * Returns true if the window is open, false otherwise.
 *
 * @return A boolean value.
 */
bool Window::isOpen()
{
    return window.isOpen();
}

/**
 * If the window is closed, return 255. If a key is pressed, return the key code. Otherwise, return 0
 *
 * @return A bitset of 8 bits.
 */
int Window::getKeyPressed()
{
    sf::Event event;

    if (!window.pollEvent(event)) return 0;
    if (event.type == sf::Event::Closed) return 255;
    if (event.type == sf::Event::KeyPressed) return event.key.code;
    return 0;
}

/**
 * It draws a sprite on the window
 *
 * @param sprite The sprite to draw.
 */
void Window::draw(Sprite& sprite, int xtexture, int ytexture, int width, int height)
{
    sprite.setTextureRect(xtexture, ytexture, width, height);

    sf::Sprite* tempSprite = static_cast<sf::Sprite*>(sprite.getSprite());

    window.draw(*tempSprite);
}

/**
 * It draws a text on the window
 *
 * @param text The text to draw.
 */
void Window::draw(Text& text)
{
    sf::Text* tempText = static_cast<sf::Text*>(text.getText());

    window.draw(*tempText);
}