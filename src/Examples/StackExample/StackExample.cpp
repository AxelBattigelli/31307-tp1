/*
** Axel BATTIGELLI, 2026
** TP1 - Complément en programmation
** File description:
** Exemple de pile, modes auto et manuel
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
 * @brief Des valeurs sont empilées et dépilées au sommet d'une liste chaînée.
 */

namespace
{

    struct Step {
        bool push;
        int value;
        const char *message;
    };

    constexpr Step kSteps[] = {
        {.push = true, .value = 4, .message = "Empiler 4. Il devient le sommet."},
        {.push = true, .value = 7, .message = "Empiler 7 au-dessus de 4."},
        {.push = true, .value = 1, .message = "Empiler 1. Le sommet change."},
        {.push = true, .value = 9, .message = "Empiler 9."},
        {.push = false, .value = 0, .message = "Dépiler 9, la dernière valeur entrée."},
        {.push = true, .value = 3, .message = "Empiler 3."},
        {.push = false, .value = 0, .message = "Dépiler 3."},
        {.push = false, .value = 0, .message = "Dépiler 1. Le sommet redevient 7."},
        {.push = false, .value = 0, .message = "Dépiler 7."},
        {.push = false, .value = 0, .message = "Dépiler 4. La pile est vide."},
    };

    constexpr std::size_t kStepCount = sizeof(kSteps) / sizeof(kSteps[0]);

} // namespace

/**
 * @brief Construit l'exemple de pile avec la police de l'application.
 * @param font Police du panneau et des nœuds.
 */
StackExample::StackExample(const sf::Font &font) : Example(font) {}

/**
 * @brief Renvoie le nom affiché dans le menu.
 * @return Le libellé de menu de cet exemple.
 */
auto StackExample::name() const -> const char *
{
    return "Pile";
}

/**
 * @brief Vide la pile et arme le scénario.
 * @param mode Auto ou Manuel.
 */
void StackExample::start(Mode mode)
{
    _list.clear();
    _focus = -1;
    _message = "La pile est vide.";
    prepare(mode, kStepCount);
}

/**
 * @brief Empile ou dépile la valeur de l'étape courante.
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
 * @brief Dessine la pile avec le nœud du sommet au-dessus des autres.
 * @param target Surface qui reçoit l'exemple.
 * @param area Partie gauche de la fenêtre.
 */
void StackExample::draw(sf::RenderTarget &target, sf::FloatRect area) const
{
    const DrawArea drawn = drawPanel(target, area, font(), "Pile", stepLabel(), _message, hint());
    if (drawn.diagram.width < 40.F || drawn.diagram.height < 30.F) {
        return;
    }

    const std::vector<int> values = _list.values();
    if (values.empty()) {
        sf::Text empty(utf8("Pile vide"), font(), 18);
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

        sf::Text value(utf8(std::to_string(values[i])), font(), 20);
        value.setFillColor(active ? sf::Color::White : sf::Color(28, 34, 46));
        placeText(value, x + (width / 2.F), y + (height / 2.F), true);
        target.draw(value);

        const char *role = nullptr;
        if (i == 0) {
            role = "sommet";
        } else if (i + 1 == count) {
            role = "fond";
        }
        if (role != nullptr) {
            sf::Text label(utf8(role), font(), 14);
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
