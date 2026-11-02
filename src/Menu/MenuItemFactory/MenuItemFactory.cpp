/*
** Axel BATTIGELLI, 2026
** TP1 - Complément en programmation
** File description:
** Fabrique qui construit une ligne de menu
*/

#include "Menu/MenuItemFactory/MenuItemFactory.hpp"

#include "Menu/MenuItem/MenuItem.hpp"

#include <SFML/Graphics/Font.hpp>
#include <memory>
#include <string>

/**
 * @file MenuItemFactory.cpp
 * @brief Construit une ligne de menu standard à partir d'un nom.
 */

/**
 * @brief Crée une ligne avec un bouton Auto et un bouton Manuel.
 * @param name Libellé de la ligne.
 * @param font Police utilisée par la ligne.
 * @return La nouvelle ligne. L'appelant en est propriétaire.
 */
auto StandardMenuItemFactory::create(const std::string &name, sf::Font &font) const
    -> std::unique_ptr<MenuItem>
{
    return std::make_unique<MenuItem>(name, font);
}
