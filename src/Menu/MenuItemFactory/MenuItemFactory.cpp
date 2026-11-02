/*
** Axel BATTIGELLI, 2026
** TP1 - Complément en programmation
** File description:
** Factory that builds a menu row
*/

#include "Menu/MenuItemFactory/MenuItemFactory.hpp"

#include "Menu/MenuItem/MenuItem.hpp"

#include <SFML/Graphics/Font.hpp>
#include <memory>
#include <string>

/**
 * @file MenuItemFactory.cpp
 * @brief Builds a standard menu row from a name.
 */

/**
 * @brief Creates one row with an Auto button and a Manual button.
 * @param name Label of the row.
 * @param font Font used by the row.
 * @return The new row. The caller owns it.
 */
auto StandardMenuItemFactory::create(const std::string &name, sf::Font &font) const
    -> std::unique_ptr<MenuItem>
{
    return std::make_unique<MenuItem>(name, font);
}
