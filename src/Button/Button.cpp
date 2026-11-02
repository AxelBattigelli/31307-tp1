/*
** Axel BATTIGELLI, 2026
** TP1 - Complément en programmation
** File description:
** Implémentation de la classe Button
*/

#include "Button/Button.hpp"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/String.hpp>
#include <algorithm>
#include <string>

/**
 * @file Button.cpp
 * @brief Libellé cliquable utilisé par une ligne de menu.
 */

/**
 * @brief Construit un bouton et lui donne ses couleurs au repos.
 * @param label Texte dessiné dans le bouton.
 * @param font Police de ce texte. Elle doit survivre au bouton.
 */
Button::Button(const std::string &label, sf::Font &font)
    : _label(sf::String::fromUtf8(label.begin(), label.end()), font, 16),
      _idleColor(sf::Color(58, 66, 84)), _hoverColor(sf::Color(78, 88, 110)),
      _selectedColor(sf::Color(70, 128, 196))
{
    _label.setFillColor(sf::Color::White);
    _shape.setOutlineThickness(1.F);
    _shape.setOutlineColor(sf::Color(90, 102, 128));
    refreshColor();
}

/**
 * @brief Place le bouton et centre son libellé dans le cadre.
 * @param x Bord gauche, en pixels.
 * @param y Bord haut, en pixels.
 * @param width Largeur du cadre. Le libellé rétrécit s'il ne tient pas.
 * @param height Hauteur du cadre. Elle fixe aussi la taille de caractère de départ.
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
 * @brief Indique si un point se trouve dans le bouton.
 * @param point Position dans les mêmes coordonnées que layout().
 * @return Vrai si le point touche le rectangle du bouton.
 */
auto Button::contains(sf::Vector2f point) const -> bool
{
    return _shape.getGlobalBounds().contains(point);
}

/**
 * @brief Marque le bouton comme celui actuellement utilisé.
 * @param selected Vrai pour le bouton actif, faux pour les autres.
 */
void Button::setSelected(bool selected)
{
    _selected = selected;
    refreshColor();
}

/**
 * @brief Suit le pointeur tant qu'il est sur le bouton.
 * @param hovered Vrai tant que le pointeur reste dans le rectangle.
 */
void Button::setHovered(bool hovered)
{
    _hovered = hovered;
    refreshColor();
}

/**
 * @brief Dessine le rectangle et son libellé.
 * @param target Fenêtre ou texture qui reçoit le bouton.
 */
void Button::draw(sf::RenderTarget &target) const
{
    target.draw(_shape);
    target.draw(_label);
}

/**
 * @brief Choisit la couleur de remplissage selon l'état courant.
 *
 * La sélection l'emporte sur le survol. Sinon le bouton reste au repos.
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
