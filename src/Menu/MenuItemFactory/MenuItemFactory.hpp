/*
** Axel BATTIGELLI, 2026
** TP1 - Complément en programmation
** File description:
** Factory that builds a menu row
*/

#pragma once

#include <SFML/Graphics/Font.hpp>
#include <memory>
#include <string>

class MenuItem;

// Factory Method: Menu does not build a row itself.
// The factory assembles the name, the auto button and the manual button.
class MenuItemFactory {
  public:
    virtual ~MenuItemFactory() = default;

    virtual auto create(const std::string &name, sf::Font &font) const
        -> std::unique_ptr<MenuItem> = 0;
};

class StandardMenuItemFactory : public MenuItemFactory {
  public:
    auto create(const std::string &name, sf::Font &font) const
        -> std::unique_ptr<MenuItem> override;
};
