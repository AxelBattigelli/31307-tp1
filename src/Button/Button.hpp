/*
** Axel BATTIGELLI, 2026
** TP1 - Complément en programmation
** File description:
** Button class declaration
*/

#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>
#include <string>

class Button {
  public:
    Button(const std::string &label, sf::Font &font);

    void layout(float x, float y, float width, float height);
    auto contains(sf::Vector2f point) const -> bool;

    void setSelected(bool selected);
    void setHovered(bool hovered);

    void draw(sf::RenderTarget &target) const;

  private:
    void refreshColor();

    sf::RectangleShape _shape;
    sf::Text _label;
    sf::Color _idleColor;
    sf::Color _hoverColor;
    sf::Color _selectedColor;
    bool _selected = false;
    bool _hovered = false;
};
