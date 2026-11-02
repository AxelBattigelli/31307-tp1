/*
** Axel BATTIGELLI, 2026
** TP1 - Complément en programmation
** File description:
** Button class implementation
*/

#include "Button/Button.hpp"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <algorithm>
#include <string>

/**
 * @file Button.cpp
 * @brief Clickable label used by a menu row.
 */

/**
 * @brief Builds a button and gives it its idle colors.
 * @param label Text drawn inside the button.
 * @param font Font used for that text. It must outlive the button.
 */
Button::Button(const std::string &label, sf::Font &font)
    : _label(label, font, 16), _idleColor(sf::Color(58, 66, 84)),
      _hoverColor(sf::Color(78, 88, 110)), _selectedColor(sf::Color(70, 128, 196))
{
    _label.setFillColor(sf::Color::White);
    _shape.setOutlineThickness(1.F);
    _shape.setOutlineColor(sf::Color(90, 102, 128));
    refreshColor();
}

/**
 * @brief Places the button and centers its label inside the box.
 * @param x Left edge, in pixels.
 * @param y Top edge, in pixels.
 * @param width Box width. The label shrinks if it does not fit.
 * @param height Box height. It also sets the starting character size.
 */
void Button::layout(float x, float y, float width, float height)
{
    _shape.setPosition(x, y);
    _shape.setSize({width, height});

    unsigned characterSize = static_cast<unsigned>(std::max(12.F, height * 0.34F));
    _label.setCharacterSize(characterSize);
    while (characterSize > 11 && _label.getLocalBounds().width > width - 10.F) {
        --characterSize;
        _label.setCharacterSize(characterSize);
    }

    const sf::FloatRect bounds = _label.getLocalBounds();
    _label.setOrigin(bounds.left + (bounds.width / 2.F), bounds.top + (bounds.height / 2.F));
    _label.setPosition(x + (width / 2.F), y + (height / 2.F));
}

/**
 * @brief Tells whether a point lies inside the button.
 * @param point Position in the same coordinates as layout().
 * @return True when the point hits the button rectangle.
 */
auto Button::contains(sf::Vector2f point) const -> bool
{
    return _shape.getGlobalBounds().contains(point);
}

/**
 * @brief Marks the button as the one currently in use.
 * @param selected True for the active button, false for the others.
 */
void Button::setSelected(bool selected)
{
    _selected = selected;
    refreshColor();
}

/**
 * @brief Tracks the pointer while it is over the button.
 * @param hovered True while the pointer stays inside the rectangle.
 */
void Button::setHovered(bool hovered)
{
    _hovered = hovered;
    refreshColor();
}

/**
 * @brief Draws the rectangle and its label.
 * @param target Window or texture that receives the button.
 */
void Button::draw(sf::RenderTarget &target) const
{
    target.draw(_shape);
    target.draw(_label);
}

/**
 * @brief Picks the fill color from the current state.
 *
 * Selection wins over hover. Otherwise the button stays idle.
 */
void Button::refreshColor()
{
    if (_selected) {
        _shape.setFillColor(_selectedColor);
    } else if (_hovered) {
        _shape.setFillColor(_hoverColor);
    } else {
        _shape.setFillColor(_idleColor);
    }
}
