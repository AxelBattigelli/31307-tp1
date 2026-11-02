/*
** Axel BATTIGELLI, 2026
** TP1 - Complément en programmation
** File description:
** main file for the project
*/

#include "Examples/Display/Display.hpp"
#include "Examples/Example/Example.hpp"
#include "Examples/QueueExample/QueueExample.hpp"
#include "Examples/StackExample/StackExample.hpp"
#include "Examples/TreeBuild/TreeBuild.hpp"
#include "Examples/TreeSearch/TreeSearch.hpp"
#include "Menu/Menu/Menu.hpp"
#include "Menu/MenuItem/MenuItem.hpp"
#include "Menu/MenuItemFactory/MenuItemFactory.hpp"
#include "SystemFont/SystemFont.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/WindowStyle.hpp>
#include <memory>
#include <string>
#include <vector>

/**
 * @file main.cpp
 * @brief Opens the window and runs the example chosen in the menu.
 */

/**
 * @brief Starts the graphical application.
 * @return 0 when the window closes, 1 when assets/OpenSans.ttf is missing.
 */
auto main() -> int
{
    constexpr unsigned int width = 1920;
    constexpr unsigned int height = 1080;

    sf::RenderWindow window(sf::VideoMode(width, height), "TP1", sf::Style::Default);
    window.setVerticalSyncEnabled(true);
    window.setKeyRepeatEnabled(false);

    SystemFont systemFont;
    if (!systemFont.load()) {
        return 1;
    }

    Menu menu(std::make_unique<StandardMenuItemFactory>(), systemFont.font());

    std::vector<std::unique_ptr<Example>> examples;
    examples.push_back(std::make_unique<QueueExample>(systemFont.font()));
    examples.push_back(std::make_unique<StackExample>(systemFont.font()));
    examples.push_back(std::make_unique<TreeBuild>(systemFont.font()));
    examples.push_back(std::make_unique<TreeSearch>(systemFont.font()));

    menu.resize(static_cast<float>(width), static_cast<float>(height));
    for (const auto &example : examples) {
        menu.add(example->name());
    }

    Example *current = nullptr;
    menu.setAction([&](const std::string &name, MenuItem::Mode mode) -> void {
        for (const auto &example : examples) {
            if (name == example->name()) {
                current = example.get();
                current->start(
                    mode == MenuItem::Mode::Auto ? Example::Mode::Auto : Example::Mode::Manual);
                break;
            }
        }
    });

    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::Resized) {
                const auto windowWidth = static_cast<float>(event.size.width);
                const auto windowHeight = static_cast<float>(event.size.height);
                window.setView(sf::View(sf::FloatRect(0.F, 0.F, windowWidth, windowHeight)));
                menu.resize(windowWidth, windowHeight);
            } else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space
                && current != nullptr) {
                current->nextStep();
            }

            menu.handleEvent(event, window);
        }

        const float dt = clock.restart().asSeconds();
        if (current != nullptr) {
            current->update(dt);
        }

        const sf::Vector2f size(window.getSize());
        const sf::FloatRect area(0.F, 0.F, size.x * 2.F / 3.F, size.y);
        window.clear(sf::Color(236, 239, 244));
        if (current != nullptr) {
            current->draw(window, area);
        } else {
            drawPanel(window, area, systemFont.font(), "Examples", "",
                "Two linked lists and two binary trees.", "Pick an example, then Auto or Manual.");
        }
        menu.draw(window);
        window.display();
    }

    return 0;
}
