/*
** Axel BATTIGELLI, 2026
** TP1 - Complément en programmation
** File description:
** Implémentation d'une ligne de menu
*/

#include "Menu/MenuItem/MenuItem.hpp"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/String.hpp>
#include <algorithm>
#include <optional>
#include <string>
#include <utility>

/**
 * @file MenuItem.cpp
 * @brief Une ligne de menu : un nom, un bouton Auto et un bouton Manuel.
 */

/**
 * @brief Construit une ligne avec les deux boutons de mode.
 * @param name Texte affiché à gauche de la ligne.
 * @param font Police partagée par le libellé et les boutons.
 */
MenuItem::MenuItem(std::string name, sf::Font &font)
    : _name(std::move(name)), _label(sf::String::fromUtf8(_name.begin(), _name.end()), font, 18),
      _autoButton("Auto", font), _manualButton("Manuel", font)
{
    _label.setFillColor(sf::Color(240, 242, 245));
    _row.setFillColor(sf::Color(40, 48, 64));
}

/**
 * @brief Place le nom et les deux boutons dans la ligne.
 * @param x Bord gauche de la ligne.
 * @param y Bord haut de la ligne.
 * @param width Largeur totale disponible pour la ligne.
 * @param height Hauteur totale disponible pour la ligne.
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
 * @brief Surligne un bouton tant que le pointeur est dessus.
 * @param mouse Position du pointeur en coordonnées de fenêtre.
 */
void MenuItem::updateHover(sf::Vector2f mouse)
{
    _autoButton.setHovered(_autoButton.contains(mouse));
    _manualButton.setHovered(_manualButton.contains(mouse));
}

/**
 * @brief Trouve quel bouton de mode contient le pointeur.
 * @param mouse Position du pointeur en coordonnées de fenêtre.
 * @return Le mode choisi, ou rien si le clic a manqué les deux boutons.
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
 * @brief Ne laisse en surbrillance que le bouton du mode donné.
 * @param mode Mode actif, ou rien pour effacer les deux boutons.
 */
void MenuItem::setActiveMode(std::optional<Mode> mode)
{
    _autoButton.setSelected(mode == Mode::Auto);
    _manualButton.setSelected(mode == Mode::Manual);
}

/**
 * @brief Renvoie le nom affiché sur la ligne.
 * @return Le nom de la ligne.
 */
auto MenuItem::name() const -> const std::string &
{
    return _name;
}

/**
 * @brief Dessine le fond de la ligne, le nom et les deux boutons.
 * @param target Fenêtre ou texture qui reçoit la ligne.
 */
void MenuItem::draw(sf::RenderTarget &target) const
{
    target.draw(_row);
    target.draw(_label);
    _autoButton.draw(target);
    _manualButton.draw(target);
}
