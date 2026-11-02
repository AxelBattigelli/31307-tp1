/*
** Axel BATTIGELLI, 2026
** TP1 - Complément en programmation
** File description:
** Step by step binary tree construction
*/

#include "Examples/TreeBuild/TreeBuild.hpp"

#include "Examples/BinaryTree/BinaryTree.hpp"
#include "Examples/Display/Display.hpp"
#include "Examples/Example/Example.hpp"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <string>

/**
 * @file TreeBuild.cpp
 * @brief Inserts values into a binary search tree one comparison at a time.
 */

namespace
{
    constexpr int kValues[] = {8, 3, 10, 1, 6, 14, 4};
}

/**
 * @brief Builds the tree-construction example with the application font.
 * @param font Font used for the panel and the nodes.
 */
TreeBuild::TreeBuild(const sf::Font &font) : Example(font) {}

/**
 * @brief Returns the name shown in the menu.
 * @return The menu label of this example.
 */
auto TreeBuild::name() const -> const char *
{
    return "Binary tree";
}

/**
 * @brief Clears the visible tree and prepares the insertion steps.
 * @param mode Auto or Manual.
 */
void TreeBuild::start(Mode mode)
{
    _steps.clear();
    _tree.clear();
    _cursor = nullptr;
    _active = nullptr;
    _message = "The tree is empty.";
    buildScenario();
    prepare(mode, _steps.size());
}

/**
 * @brief Records every comparison and insertion for the fixed values.
 */
void TreeBuild::buildScenario()
{
    _steps.clear();
    BinaryTree simulation;
    for (int const value : kValues) {
        addSteps(simulation, value);
    }
}

/**
 * @brief Walks a temporary tree and stores one step per comparison.
 * @param simulation Tree that already contains the previous values.
 * @param value Value whose path is recorded.
 */
void TreeBuild::addSteps(BinaryTree &simulation, int value)
{
    if (simulation.empty()) {
        simulation.addRoot(value);
        _steps.push_back(Step{.kind = StepKind::Root,
            .fromRoot = false,
            .left = false,
            .value = value,
            .message = std::to_string(value) + " becomes the root."});
        return;
    }

    bool first = true;
    TreeNode *current = simulation.root();
    while (current != nullptr) {
        if (value < current->value) {
            if (!current->left) {
                simulation.addChild(*current, true, value);
                _steps.push_back(Step{.kind = StepKind::Place,
                    .fromRoot = first,
                    .left = true,
                    .value = value,
                    .message = std::to_string(value) + " < " + std::to_string(current->value)
                        + ": place it on the left."});
                return;
            }
            _steps.push_back(Step{.kind = StepKind::Descend,
                .fromRoot = first,
                .left = true,
                .value = current->left->value,
                .message = std::to_string(value) + " < " + std::to_string(current->value)
                    + ": go down toward " + std::to_string(current->left->value) + "."});
            current = current->left.get();
        } else if (value > current->value) {
            if (!current->right) {
                simulation.addChild(*current, false, value);
                _steps.push_back(Step{.kind = StepKind::Place,
                    .fromRoot = first,
                    .left = false,
                    .value = value,
                    .message = std::to_string(value) + " > " + std::to_string(current->value)
                        + ": place it on the right."});
                return;
            }
            _steps.push_back(Step{.kind = StepKind::Descend,
                .fromRoot = first,
                .left = false,
                .value = current->right->value,
                .message = std::to_string(value) + " > " + std::to_string(current->value)
                    + ": go down toward " + std::to_string(current->right->value) + "."});
            current = current->right.get();
        } else {
            return;
        }
        first = false;
    }
}

/**
 * @brief Replays one recorded step on the tree shown on screen.
 */
void TreeBuild::applyStep()
{
    const Step &step = _steps[index()];
    if (step.kind == StepKind::Root) {
        _tree.addRoot(step.value);
        _active = _tree.root();
    } else {
        if (step.fromRoot) {
            _cursor = _tree.root();
        }
        if (step.kind == StepKind::Descend) {
            _active = _cursor;
            _cursor = step.left ? _cursor->left.get() : _cursor->right.get();
        } else {
            _active = _tree.addChild(*_cursor, step.left, step.value);
        }
    }
    _message = step.message;
}

/**
 * @brief Draws the tree built so far and highlights the current node.
 * @param target Surface that receives the example.
 * @param area Left part of the window.
 */
void TreeBuild::draw(sf::RenderTarget &target, sf::FloatRect area) const
{
    const DrawArea drawn =
        drawPanel(target, area, font(), "Binary tree", stepLabel(), _message, hint());
    drawTree(target, font(), drawn.diagram, _tree.root(), _active);
}
