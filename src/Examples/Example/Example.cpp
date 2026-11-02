/*
** Axel BATTIGELLI, 2026
** TP1 - Complément en programmation
** File description:
** Shared auto and manual step logic
*/

#include "Examples/Example/Example.hpp"

#include <SFML/Graphics/Font.hpp>
#include <cstddef>
#include <string>

/**
 * @file Example.cpp
 * @brief Clock and step counter shared by every example.
 */

namespace
{
    constexpr float kAutoDelay = 1.15F;
}

/**
 * @brief Stores the font used by the example texts.
 * @param font Font loaded by the menu. It must outlive the example.
 */
Example::Example(const sf::Font &font) : _font(font) {}

/**
 * @brief Resets the run before the first step.
 * @param mode Auto runs on a timer, Manual waits for the space bar.
 * @param total Number of steps in the scenario.
 */
void Example::prepare(Mode mode, std::size_t total)
{
    _mode = mode;
    _started = true;
    _finished = total == 0;
    _index = 0;
    _total = total;
    _elapsed = 0.F;
}

/**
 * @brief Advances one step in Auto mode when the delay has elapsed.
 * @param dt Seconds since the previous frame.
 */
void Example::update(float dt)
{
    if (!_started || _finished || _mode != Mode::Auto) {
        return;
    }

    _elapsed += dt;
    if (_elapsed >= kAutoDelay) {
        _elapsed = 0.F;
        runStep();
    }
}

/**
 * @brief Advances one step when Manual mode is running.
 */
void Example::nextStep()
{
    if (!_started || _finished || _mode != Mode::Manual) {
        return;
    }
    runStep();
}

/**
 * @brief Gives the font stored at construction.
 * @return The font used to draw the example.
 */
auto Example::font() const -> const sf::Font &
{
    return _font;
}

/**
 * @brief Returns how many steps have already been applied.
 * @return The current step index, starting at zero.
 */
auto Example::index() const -> std::size_t
{
    return _index;
}

/**
 * @brief Formats the step counter shown above the diagram.
 * @return A label such as "Step 2 / 8", or an empty string before the start.
 */
auto Example::stepLabel() const -> std::string
{
    if (!_started) {
        return "";
    }
    return "Step " + std::to_string(_index) + " / " + std::to_string(_total);
}

/**
 * @brief Describes what the user can do in the current state.
 * @return A short hint for the bottom of the panel.
 */
auto Example::hint() const -> const char *
{
    if (!_started) {
        return "Pick Auto or Manual in the menu.";
    }
    if (_finished) {
        return "Done. Start it again from the menu.";
    }
    if (_mode == Mode::Auto) {
        return "Auto mode: the steps run on their own.";
    }
    return "Manual mode: press space for the next step.";
}

/**
 * @brief Applies the current step and stops when the scenario is over.
 */
void Example::runStep()
{
    if (!_started || _finished || _index >= _total) {
        _finished = true;
        return;
    }

    applyStep();
    ++_index;
    if (_index >= _total) {
        _finished = true;
    }
}
