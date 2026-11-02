/*
** Axel BATTIGELLI, 2026
** TP1 - Complément en programmation
** File description:
** Fabrique qui construit une ligne de menu
*/

#pragma once

#include <SFML/Graphics/Font.hpp>
#include <memory>
#include <string>

class MenuItem;

// Méthode fabrique : Menu ne construit pas lui-même une ligne.
// La fabrique assemble le nom, le bouton auto et le bouton manuel.
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
