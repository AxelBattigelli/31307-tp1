/*
** Axel BATTIGELLI, 2026
** TP1 - Complément en programmation
** File description:
** Queue example, auto and manual modes
*/

#include "Examples/QueueExample/QueueExample.hpp"

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
 * @file QueueExample.cpp
 * @brief Customers join the back of a linked list and leave from the front.
 */

namespace
{

    struct Step {
        bool insert;
        int value;
        const char *message;
    };

    constexpr Step kSteps[] = {
        {.insert = true, .value = 4, .message = "Customer 4 joins the back of the queue."},
        {.insert = true, .value = 9, .message = "Customer 9 joins the back of the queue."},
        {.insert = true, .value = 2, .message = "Customer 2 joins the back of the queue."},
        {.insert = false, .value = 0, .message = "Customer 4 is served: the front leaves."},
        {.insert = true, .value = 7, .message = "Customer 7 joins the back of the queue."},
        {.insert = false, .value = 0, .message = "Customer 9 is served: the front leaves."},
        {.insert = false, .value = 0, .message = "Customer 2 is served: the front leaves."},
        {.insert = false, .value = 0, .message = "Customer 7 is served: the front leaves."},
    };

    constexpr std::size_t kStepCount = sizeof(kSteps) / sizeof(kSteps[0]);

} // namespace

/**
 * @brief Builds the queue example with the menu font.
 * @param font Font used for the panel and the nodes.
 */
QueueExample::QueueExample(const sf::Font &font) : Example(font) {}

/**
 * @brief Returns the name shown in the menu.
 * @return The menu label of this example.
 */
auto QueueExample::name() const -> const char *
{
    return "Queue";
}

/**
 * @brief Clears the queue and arms the scenario.
 * @param mode Auto or Manual.
 */
void QueueExample::start(Mode mode)
{
    _list.clear();
    _focus = -1;
    _message = "The queue is empty.";
    prepare(mode, kStepCount);
}

/**
 * @brief Enqueues or dequeues the customer of the current step.
 */
void QueueExample::applyStep()
{
    const Step &step = kSteps[index()];
    if (step.insert) {
        _list.enqueue(step.value);
        _focus = static_cast<int>(_list.size()) - 1;
    } else {
        _list.dequeue();
        _focus = _list.empty() ? -1 : 0;
    }
    _message = step.message;
}

/**
 * @brief Draws the queue from the front to the back.
 * @param target Surface that receives the example.
 * @param area Left part of the window.
 */
void QueueExample::draw(sf::RenderTarget &target, sf::FloatRect area) const
{
    const DrawArea drawn = drawPanel(target, area, font(), "Queue", stepLabel(), _message, hint());
    if (drawn.diagram.width < 40.F || drawn.diagram.height < 30.F) {
        return;
    }

    const std::vector<int> values = _list.values();
    const float centerY = drawn.diagram.top + (drawn.diagram.height / 2.F);

    if (values.empty()) {
        sf::Text empty("No node", font(), 18);
        empty.setFillColor(sf::Color(140, 148, 162));
        placeText(empty, drawn.diagram.left + (drawn.diagram.width / 2.F), centerY, true);
        target.draw(empty);
        return;
    }

    const float available = drawn.diagram.width - 8.F;
    float width = 76.F;
    float gap = 46.F;
    const std::size_t count = values.size();
    float needed = (count * width) + ((count - 1) * gap);
    if (needed > available) {
        gap = 24.F;
        width = std::max(48.F, (available - (count - 1) * gap) / static_cast<float>(count));
        needed = (count * width) + ((count - 1) * gap);
    }

    const float height = 54.F;
    float x = drawn.diagram.left + ((drawn.diagram.width - needed) / 2.F);

    for (std::size_t i = 0; i < count; ++i) {
        const bool active = std::cmp_equal(i, _focus);
        sf::RectangleShape box({width, height});
        box.setPosition(x, centerY - (height / 2.F));
        box.setFillColor(active ? sf::Color(70, 128, 196) : sf::Color(244, 246, 250));
        box.setOutlineThickness(2.F);
        box.setOutlineColor(active ? sf::Color(70, 128, 196) : sf::Color(180, 188, 202));
        target.draw(box);

        sf::Text value(std::to_string(values[i]), font(), 22);
        value.setFillColor(active ? sf::Color::White : sf::Color(28, 34, 46));
        placeText(value, x + (width / 2.F), centerY, true);
        target.draw(value);

        std::string role;
        if (count == 1) {
            role = "front / back";
        } else if (i == 0) {
            role = "front";
        } else if (i + 1 == count) {
            role = "back";
        }

        if (!role.empty()) {
            sf::Text label(role, font(), 14);
            label.setFillColor(sf::Color(96, 108, 128));
            placeText(label, x + (width / 2.F), centerY + (height / 2.F) + 18.F, true);
            target.draw(label);
        }

        if (i + 1 < count) {
            drawRightArrow(target, x + width + 4.F, x + width + gap - 4.F, centerY);
        }
        x += width + gap;
    }
}
