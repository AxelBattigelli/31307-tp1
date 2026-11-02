/*
** Axel BATTIGELLI, 2026
** TP1 - Complément en programmation
** File description:
** Implémentation de la classe Menu
*/

#include "Menu/Menu/Menu.hpp"

#include "Menu/MenuItem/MenuItem.hpp"
#include "Menu/MenuItemFactory/MenuItemFactory.hpp"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>
#include <algorithm>
#include <memory>
#include <optional>
#include <string>
#include <utility>

/**
 * @file Menu.cpp
 * @brief Menu à droite, qui occupe un tiers de la fenêtre.
 */

/**
 * @brief Construit un menu vide dont les lignes passent par une fabrique.
 * @param factory Construit chaque ligne. Le menu en prend possession.
 * @param font Police partagée par toute l'application. Elle doit survivre au menu.
 */
Menu::Menu(std::unique_ptr<MenuItemFactory> factory, sf::Font &font)
    : _factory(std::move(factory)), _font(font)
{
    _background.setFillColor(sf::Color(28, 34, 46));
    _separator.setFillColor(sf::Color(70, 82, 104));
    _title.setFont(_font);
    _title.setString("Menu");
    _title.setFillColor(sf::Color(230, 233, 240));
}

/**
 * @brief Détruit le menu et les lignes qu'il possède.
 */
Menu::~Menu() = default;

/**
 * @brief Ajoute une ligne construite par la fabrique.
 * @param name Libellé affiché à gauche de la ligne.
 */
void Menu::add(const std::string &name)
{
    if (!_factory) {
        return;
    }

    _items.push_back(_factory->create(name, _font));
    layout();
}

/**
 * @brief Mémorise le rappel déclenché quand un bouton de mode est pressé.
 * @param action Reçoit le nom de la ligne et le mode choisi.
 */
void Menu::setAction(Action action)
{
    _action = std::move(action);
}

/**
 * @brief Garde le menu sur le tiers droit après un redimensionnement.
 * @param windowWidth Nouvelle largeur de la fenêtre, en pixels.
 * @param windowHeight Nouvelle hauteur de la fenêtre, en pixels.
 */
void Menu::resize(float windowWidth, float windowHeight)
{
    if (windowWidth < 1.F || windowHeight < 1.F) {
        return;
    }

    _windowWidth = windowWidth;
    _windowHeight = windowHeight;
    layout();
}

/**
 * @brief Met à jour le survol et lance l'action du bouton de mode cliqué.
 * @param event Événement pris dans la fenêtre.
 * @param window Fenêtre utilisée pour convertir la position de la souris.
 */
void Menu::handleEvent(const sf::Event &event, const sf::RenderWindow &window)
{
    if (event.type == sf::Event::MouseMoved) {
        const sf::Vector2f mouse = window.mapPixelToCoords({event.mouseMove.x, event.mouseMove.y});
        for (const auto &item : _items) {
            item->updateHover(mouse);
        }
    } else if (event.type == sf::Event::MouseButtonPressed
        && event.mouseButton.button == sf::Mouse::Left) {
        const sf::Vector2f mouse =
            window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});
        for (const auto &item : _items) {
            const std::optional<MenuItem::Mode> mode = item->hit(mouse);
            if (!mode) {
                continue;
            }

            for (const auto &other : _items) {
                other->setActiveMode(other.get() == item.get() ? mode : std::nullopt);
            }

            if (_action) {
                _action(item->name(), *mode);
            }
            break;
        }
    }
}

/**
 * @brief Dessine le panneau, le titre et chaque ligne.
 * @param target Fenêtre ou texture qui reçoit le menu.
 */
void Menu::draw(sf::RenderTarget &target) const
{
    target.draw(_background);
    target.draw(_title);
    target.draw(_separator);
    for (const auto &item : _items) {
        item->draw(target);
    }
}

/**
 * @brief Place le panneau et empile les lignes de haut en bas.
 */
void Menu::layout()
{
    const float menuWidth = _windowWidth / 3.F;
    const float menuX = _windowWidth - menuWidth;
    const float padding = 16.F;

    _background.setPosition(menuX, 0.F);
    _background.setSize({menuWidth, _windowHeight});

    const unsigned titleSize = static_cast<unsigned>(std::max(16.F, _windowHeight * 0.035F));
    _title.setCharacterSize(titleSize);
    const sf::FloatRect titleBounds = _title.getLocalBounds();
    _title.setOrigin(titleBounds.left, titleBounds.top);
    _title.setPosition(menuX + padding, padding);

    const float separatorY = padding + titleBounds.height + 12.F;
    _separator.setPosition(menuX + padding, separatorY);
    _separator.setSize({std::max(0.F, menuWidth - (2.F * padding)), 1.F});

    const float rowHeight = std::clamp(_windowHeight * 0.08F, 40.F, 56.F);
    const float gap = 10.F;
    const float firstRowY = _separator.getPosition().y + 16.F;

    float y = firstRowY;
    for (const auto &item : _items) {
        item->layout(menuX + padding, y, std::max(0.F, menuWidth - (2.F * padding)), rowHeight);
        y += rowHeight + gap;
    }
}
