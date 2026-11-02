/*
** Axel BATTIGELLI, 2026
** TP1 - Complément en programmation
** File description:
** Menu row implementation
*/

#include "Menu/MenuItem/MenuItem.hpp"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <algorithm>
#include <optional>
#include <string>
#include <utility>

/**
 * @file MenuItem.cpp
 * @brief One menu row: a name, an Auto button and a Manual button.
 */

/**
 * @brief Builds a row with both mode buttons.
 * @param name Text shown on the left of the row.
 * @param font Font shared by the label and the buttons.
 */
MenuItem::MenuItem(std::string name, sf::Font &font)
    : _name(std::move(name)), _label(_name, font, 18), _autoButton("Auto", font),
      _manualButton("Manual", font)
{
    _label.setFillColor(sf::Color(240, 242, 245));
    _row.setFillColor(sf::Color(40, 48, 64));
}

/**
 * @brief Places the name and the two buttons inside the row.
 * @param x Left edge of the row.
 * @param y Top edge of the row.
 * @param width Full width available for the row.
 * @param height Full height available for the row.
 */
void MenuItem::layout(float x, float y, float width, float height)
{
    _row.setPosition(x, y);
    _row.setSize({width, height});

    const float gap = 8.F;
    const float buttonWidth = std::clamp(width * 0.22F, 68.F, 96.F);
    const float nameWidth = std::max(0.F, width - (buttonWidth * 2.F) - (gap * 2.F));

    unsigned characterSize = static_cast<unsigned>(std::max(12.F, height * 0.34F));
    _label.setCharacterSize(characterSize);
    while (characterSize > 12 && _label.getLocalBounds().width > nameWidth - 16.F) {
        --characterSize;
        _label.setCharacterSize(characterSize);
    }

    const sf::FloatRect bounds = _label.getLocalBounds();
    _label.setOrigin(bounds.left, bounds.top + (bounds.height / 2.F));
    _label.setPosition(x + 12.F, y + (height / 2.F));

    const float buttonsX = x + nameWidth + gap;
    _autoButton.layout(buttonsX, y, buttonWidth, height);
    _manualButton.layout(buttonsX + buttonWidth + gap, y, buttonWidth, height);
}

/**
 * @brief Highlights a button while the pointer is over it.
 * @param mouse Pointer position in window coordinates.
 */
void MenuItem::updateHover(sf::Vector2f mouse)
{
    _autoButton.setHovered(_autoButton.contains(mouse));
    _manualButton.setHovered(_manualButton.contains(mouse));
}

/**
 * @brief Finds which mode button contains the pointer.
 * @param mouse Pointer position in window coordinates.
 * @return The chosen mode, or nothing when the click missed both buttons.
 */
auto MenuItem::hit(sf::Vector2f mouse) const -> std::optional<MenuItem::Mode>
{
    if (_autoButton.contains(mouse)) {
        return Mode::Auto;
    }
    if (_manualButton.contains(mouse)) {
        return Mode::Manual;
    }
    return std::nullopt;
}

/**
 * @brief Leaves only the given mode button highlighted.
 * @param mode Active mode, or nothing to clear both buttons.
 */
void MenuItem::setActiveMode(std::optional<Mode> mode)
{
    _autoButton.setSelected(mode == Mode::Auto);
    _manualButton.setSelected(mode == Mode::Manual);
}

/**
 * @brief Returns the name shown on the row.
 * @return The row name.
 */
auto MenuItem::name() const -> const std::string &
{
    return _name;
}

/**
 * @brief Draws the row background, the name and both buttons.
 * @param target Window or texture that receives the row.
 */
void MenuItem::draw(sf::RenderTarget &target) const
{
    target.draw(_row);
    target.draw(_label);
    _autoButton.draw(target);
    _manualButton.draw(target);
}
