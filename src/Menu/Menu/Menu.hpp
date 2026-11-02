/*
** Axel BATTIGELLI, 2026
** TP1 - Complément en programmation
** File description:
** Déclaration de la classe Menu
*/

#pragma once

#include "Menu/MenuItem/MenuItem.hpp"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Event.hpp>
#include <functional>
#include <memory>
#include <string>
#include <vector>

class MenuItemFactory;

class Menu {
  public:
    Menu(std::unique_ptr<MenuItemFactory> factory, sf::Font &font);
    ~Menu();

    Menu(const Menu &) = delete;
    auto operator=(const Menu &) -> Menu & = delete;
    Menu(Menu &&) = delete;
    auto operator=(Menu &&) -> Menu & = delete;

    using Action = std::function<void(const std::string &name, MenuItem::Mode mode)>;

    void add(const std::string &name);
    void setAction(Action action);
    void resize(float windowWidth, float windowHeight);
    void handleEvent(const sf::Event &event, const sf::RenderWindow &window);
    void draw(sf::RenderTarget &target) const;

  private:
    void layout();

    std::unique_ptr<MenuItemFactory> _factory;
    std::vector<std::unique_ptr<MenuItem>> _items;
    sf::Font &_font;
    sf::Text _title;
    sf::RectangleShape _background;
    sf::RectangleShape _separator;
    float _windowWidth = 0.F;
    float _windowHeight = 0.F;
    Action _action;
};
