/*
** Axel BATTIGELLI, 2026
** TP1 - Complément en programmation
** File description:
** Stack example, auto and manual modes
*/

#include "Examples/StackExample/StackExample.hpp"

#include "Examples/Display/Display.hpp"
#include "Examples/Example/Example.hpp"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>
#include <algorithm>
#include <cstddef>
#include <string>
#include <utility>
#include <vector>

/**
 * @file StackExample.cpp
 * @brief Values are pushed and popped at the top of a linked list.
 */

namespace
{

    struct Step {
        bool push;
        int value;
        const char *message;
    };

    constexpr Step kSteps[] = {
        {.push = true, .value = 4, .message = "Push 4. It becomes the top."},
        {.push = true, .value = 7, .message = "Push 7 above 4."},
        {.push = true, .value = 1, .message = "Push 1. The top changes."},
        {.push = true, .value = 9, .message = "Push 9."},
        {.push = false, .value = 0, .message = "Pop 9, the last value in."},
        {.push = true, .value = 3, .message = "Push 3."},
        {.push = false, .value = 0, .message = "Pop 3."},
        {.push = false, .value = 0, .message = "Pop 1. The top is 7 again."},
        {.push = false, .value = 0, .message = "Pop 7."},
        {.push = false, .value = 0, .message = "Pop 4. The stack is empty."},
    };

    constexpr std::size_t kStepCount = sizeof(kSteps) / sizeof(kSteps[0]);

} // namespace

/**
 * @brief Builds the stack example with the application font.
 * @param font Font used for the panel and the nodes.
 */
StackExample::StackExample(const sf::Font &font) : Example(font) {}

/**
 * @brief Returns the name shown in the menu.
 * @return The menu label of this example.
 */
auto StackExample::name() const -> const char *
{
    return "Stack";
}

/**
 * @brief Clears the stack and arms the scenario.
 * @param mode Auto or Manual.
 */
void StackExample::start(Mode mode)
{
    _list.clear();
    _focus = -1;
    _message = "The stack is empty.";
    prepare(mode, kStepCount);
}

/**
 * @brief Pushes or pops the value of the current step.
 */
void StackExample::applyStep()
{
    const Step &step = kSteps[index()];
    if (step.push) {
        _list.push(step.value);
        _focus = 0;
    } else {
        _list.pop();
        _focus = _list.empty() ? -1 : 0;
    }
    _message = step.message;
}

/**
 * @brief Draws the stack with the top node above the others.
 * @param target Surface that receives the example.
 * @param area Left part of the window.
 */
void StackExample::draw(sf::RenderTarget &target, sf::FloatRect area) const
{
    const DrawArea drawn = drawPanel(target, area, font(), "Stack", stepLabel(), _message, hint());
    if (drawn.diagram.width < 40.F || drawn.diagram.height < 30.F) {
        return;
    }

    const std::vector<int> values = _list.values();
    if (values.empty()) {
        sf::Text empty("Empty stack", font(), 18);
        empty.setFillColor(sf::Color(140, 148, 162));
        placeText(empty, drawn.diagram.left + (drawn.diagram.width / 2.F),
            drawn.diagram.top + (drawn.diagram.height / 2.F), true);
        target.draw(empty);
        return;
    }

    const std::size_t count = values.size();
    float const width = 112.F;
    float height = 46.F;
    float gap = 34.F;
    float needed = (count * height) + ((count - 1) * gap);
    if (needed > drawn.diagram.height - 8.F) {
        gap = 18.F;
        height = std::max(
            36.F, (drawn.diagram.height - 8.F - (count - 1) * gap) / static_cast<float>(count));
        needed = (count * height) + ((count - 1) * gap);
    }

    const float x = drawn.diagram.left + ((drawn.diagram.width - width) / 2.F);
    float y = drawn.diagram.top + ((drawn.diagram.height - needed) / 2.F);

    for (std::size_t i = 0; i < count; ++i) {
        const bool active = std::cmp_equal(i, _focus);
        sf::RectangleShape box({width, height});
        box.setPosition(x, y);
        box.setFillColor(active ? sf::Color(70, 128, 196) : sf::Color(244, 246, 250));
        box.setOutlineThickness(2.F);
        box.setOutlineColor(active ? sf::Color(70, 128, 196) : sf::Color(180, 188, 202));
        target.draw(box);

        sf::Text value(std::to_string(values[i]), font(), 20);
        value.setFillColor(active ? sf::Color::White : sf::Color(28, 34, 46));
        placeText(value, x + (width / 2.F), y + (height / 2.F), true);
        target.draw(value);

        const char *role = nullptr;
        if (i == 0) {
            role = "top";
        } else if (i + 1 == count) {
            role = "bottom";
        }
        if (role != nullptr) {
            sf::Text label(role, font(), 14);
            label.setFillColor(sf::Color(96, 108, 128));
            const sf::FloatRect bounds = label.getLocalBounds();
            label.setOrigin(bounds.left, bounds.top + (bounds.height / 2.F));
            label.setPosition(x + width + 14.F, y + (height / 2.F));
            target.draw(label);
        }

        if (i + 1 < count) {
            drawDownArrow(target, x + (width / 2.F), y + height + 3.F, y + height + gap - 3.F);
        }
        y += height + gap;
    }
}
