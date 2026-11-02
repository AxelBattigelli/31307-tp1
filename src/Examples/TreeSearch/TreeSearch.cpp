/*
** Axel BATTIGELLI, 2026
** TP1 - Complément en programmation
** File description:
** Step by step search in a binary tree
*/

#include "Examples/TreeSearch/TreeSearch.hpp"

#include "Examples/BinaryTree/BinaryTree.hpp"
#include "Examples/Display/Display.hpp"
#include "Examples/Example/Example.hpp"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <string>

/**
 * @file TreeSearch.cpp
 * @brief Searches a ready binary tree, including values that are absent.
 */

namespace
{
    constexpr int kValues[] = {8, 3, 10, 1, 6, 14, 4};
    constexpr int kTargets[] = {6, 14, 2, 9};
} // namespace

/**
 * @brief Builds the search example with the menu font.
 * @param font Font used for the panel and the nodes.
 */
TreeSearch::TreeSearch(const sf::Font &font) : Example(font) {}

/**
 * @brief Returns the name shown in the menu.
 * @return The menu label of this example.
 */
auto TreeSearch::name() const -> const char *
{
    return "Search";
}

/**
 * @brief Builds the tree, then records the search of each target.
 * @param mode Auto or Manual.
 */
void TreeSearch::start(Mode mode)
{
    _steps.clear();
    _active = nullptr;
    _tree.clear();
    for (int const value : kValues) {
        _tree.insert(value);
    }
    for (int const target : kTargets) {
        record(target);
    }
    _message = "Tree ready. Searching for 6, 14, 2, then 9.";
    prepare(mode, _steps.size());
}

/**
 * @brief Stores one step for each comparison made while looking for a value.
 * @param target Value to search. It may be missing from the tree.
 */
void TreeSearch::record(int target)
{
    const TreeNode *node = _tree.root();
    const std::string targetText = std::to_string(target);
    bool first = true;

    while (node != nullptr) {
        const std::string here = std::to_string(node->value);
        const std::string prefix = first ? "Search for " + targetText + ". " : "";
        first = false;

        if (node->value == target) {
            _steps.push_back(
                Step{.node = node, .message = prefix + targetText + " is in the tree."});
            return;
        }

        if (target < node->value) {
            if (node->left == nullptr) {
                _steps.push_back(Step{.node = node,
                    .message = prefix + targetText + " < " + here
                        + " and the left child is empty: not found."});
                return;
            }
            _steps.push_back(
                Step{.node = node, .message = prefix + targetText + " < " + here + ": go left."});
            node = node->left.get();
        } else {
            if (node->right == nullptr) {
                _steps.push_back(Step{.node = node,
                    .message = prefix + targetText + " > " + here
                        + " and the right child is empty: not found."});
                return;
            }
            _steps.push_back(
                Step{.node = node, .message = prefix + targetText + " > " + here + ": go right."});
            node = node->right.get();
        }
    }
}

/**
 * @brief Highlights the node compared during the current search step.
 */
void TreeSearch::applyStep()
{
    const Step &step = _steps[index()];
    _active = step.node;
    _message = step.message;
}

/**
 * @brief Draws the full tree and the node of the current comparison.
 * @param target Surface that receives the example.
 * @param area Left part of the window.
 */
void TreeSearch::draw(sf::RenderTarget &target, sf::FloatRect area) const
{
    const DrawArea drawn =
        drawPanel(target, area, font(), "Search", stepLabel(), _message, hint());
    drawTree(target, font(), drawn.diagram, _tree.root(), _active);
}
