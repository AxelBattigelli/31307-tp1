/*
** Axel BATTIGELLI, 2026
** TP1 - Complément en programmation
** File description:
** Déclaration d'une ligne de menu
*/

#pragma once

#include "Button/Button.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>
#include <optional>
#include <string>

class MenuItem {
  public:
    enum class Mode { Auto, Manual };

    MenuItem(std::string name, sf::Font &font);

    void layout(float x, float y, float width, float height);
    void updateHover(sf::Vector2f mouse);
    auto hit(sf::Vector2f mouse) const -> std::optional<Mode>;
    void setActiveMode(std::optional<Mode> mode);
    void draw(sf::RenderTarget &target) const;

    auto name() const -> const std::string &;

  private:
    std::string _name;
    sf::RectangleShape _row;
    sf::Text _label;
    Button _autoButton;
    Button _manualButton;
};
