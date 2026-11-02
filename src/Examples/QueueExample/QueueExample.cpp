/*
** Axel BATTIGELLI, 2026
** TP1 - Complément en programmation
** File description:
** Exemple de file, modes auto et manuel
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
 * @brief Des clients rejoignent la fin d'une liste chaînée et partent par le début.
 */

namespace
{

    struct Step {
        bool insert;
        int value;
        const char *message;
    };

    constexpr Step kSteps[] = {
        {.insert = true, .value = 4, .message = "Le client 4 rejoint la fin de la file."},
        {.insert = true, .value = 9, .message = "Le client 9 rejoint la fin de la file."},
        {.insert = true, .value = 2, .message = "Le client 2 rejoint la fin de la file."},
        {.insert = false,
            .value = 0,
            .message = "Le client 4 est servi : la tête quitte la file."},
        {.insert = true, .value = 7, .message = "Le client 7 rejoint la fin de la file."},
        {.insert = false,
            .value = 0,
            .message = "Le client 9 est servi : la tête quitte la file."},
        {.insert = false,
            .value = 0,
            .message = "Le client 2 est servi : la tête quitte la file."},
        {.insert = false,
            .value = 0,
            .message = "Le client 7 est servi : la tête quitte la file."},
    };

    constexpr std::size_t kStepCount = sizeof(kSteps) / sizeof(kSteps[0]);

} // namespace

/**
 * @brief Construit l'exemple de file avec la police de l'application.
 * @param font Police du panneau et des nœuds.
 */
QueueExample::QueueExample(const sf::Font &font) : Example(font) {}

/**
 * @brief Renvoie le nom affiché dans le menu.
 * @return Le libellé de menu de cet exemple.
 */
auto QueueExample::name() const -> const char *
{
    return "File";
}

/**
 * @brief Vide la file et arme le scénario.
 * @param mode Auto ou Manuel.
 */
void QueueExample::start(Mode mode)
{
    _list.clear();
    _focus = -1;
    _message = "La file est vide.";
    prepare(mode, kStepCount);
}

/**
 * @brief Enfile ou défile le client de l'étape courante.
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
 * @brief Dessine la file de la tête vers la queue.
 * @param target Surface qui reçoit l'exemple.
 * @param area Partie gauche de la fenêtre.
 */
void QueueExample::draw(sf::RenderTarget &target, sf::FloatRect area) const
{
    const DrawArea drawn = drawPanel(target, area, font(), "File", stepLabel(), _message, hint());
    if (drawn.diagram.width < 40.F || drawn.diagram.height < 30.F) {
        return;
    }

    const std::vector<int> values = _list.values();
    const float centerY = drawn.diagram.top + (drawn.diagram.height / 2.F);

    if (values.empty()) {
        sf::Text empty(utf8("Aucun nœud"), font(), 18);
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

        sf::Text value(utf8(std::to_string(values[i])), font(), 22);
        value.setFillColor(active ? sf::Color::White : sf::Color(28, 34, 46));
        placeText(value, x + (width / 2.F), centerY, true);
        target.draw(value);

        std::string role;
        if (count == 1) {
            role = "tête / queue";
        } else if (i == 0) {
            role = "tête";
        } else if (i + 1 == count) {
            role = "queue";
        }

        if (!role.empty()) {
            sf::Text label(utf8(role), font(), 14);
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
