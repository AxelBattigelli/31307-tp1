/*
** Axel BATTIGELLI, 2026
** TP1 - Complément en programmation
** File description:
** Drawing helpers for the examples
*/

#pragma once

#include "Examples/BinaryTree/BinaryTree.hpp"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>
#include <string>

struct DrawArea {
    sf::FloatRect card;
    sf::FloatRect diagram;
};

void placeText(sf::Text &text, float x, float y, bool center);
void drawRightArrow(sf::RenderTarget &target, float x1, float x2, float y);
void drawDownArrow(sf::RenderTarget &target, float x, float y1, float y2);

auto drawPanel(sf::RenderTarget &target, sf::FloatRect area, const sf::Font &font,
    const std::string &title, const std::string &step, const std::string &message,
    const std::string &hint) -> DrawArea;

void drawTree(sf::RenderTarget &target, const sf::Font &font, sf::FloatRect area,
    const TreeNode *root, const TreeNode *active);
