/*
** Axel BATTIGELLI, 2026
** TP1 - Complément en programmation
** File description:
** Logique d'étapes partagée, modes auto et manuel
*/

#include "Examples/Example/Example.hpp"

#include <SFML/Graphics/Font.hpp>
#include <cstddef>
#include <string>

/**
 * @file Example.cpp
 * @brief Horloge et compteur d'étapes partagés par chaque exemple.
 */

namespace
{
    constexpr float kAutoDelay = 1.15F;
}

/**
 * @brief Mémorise la police des textes de l'exemple.
 * @param font Police chargée une fois pour l'application. Elle doit survivre à l'exemple.
 */
Example::Example(const sf::Font &font) : _font(font) {}

/**
 * @brief Réinitialise l'exécution avant la première étape.
 * @param mode Auto avance sur un minuteur, Manuel attend la barre d'espace.
 * @param total Nombre d'étapes du scénario.
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
 * @brief Avance d'une étape en mode Auto quand le délai est écoulé.
 * @param dt Secondes depuis l'image précédente.
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
 * @brief Avance d'une étape quand le mode Manuel est en cours.
 */
void Example::nextStep()
{
    if (!_started || _finished || _mode != Mode::Manual) {
        return;
    }
    runStep();
}

/**
 * @brief Donne la police mémorisée à la construction.
 * @return La police utilisée pour dessiner l'exemple.
 */
auto Example::font() const -> const sf::Font &
{
    return _font;
}

/**
 * @brief Indique combien d'étapes ont déjà été appliquées.
 * @return L'indice de l'étape courante, à partir de zéro.
 */
auto Example::index() const -> std::size_t
{
    return _index;
}

/**
 * @brief Formate le compteur d'étapes affiché au-dessus du schéma.
 * @return Une étiquette telle que « Étape 2 / 8 », ou une chaîne vide avant le départ.
 */
auto Example::stepLabel() const -> std::string
{
    if (!_started) {
        return "";
    }
    return "Étape " + std::to_string(_index) + " / " + std::to_string(_total);
}

/**
 * @brief Décrit ce que l'utilisateur peut faire dans l'état courant.
 * @return Une courte indication en bas du panneau.
 */
auto Example::hint() const -> const char *
{
    if (!_started) {
        return "Choisissez Auto ou Manuel dans le menu.";
    }
    if (_finished) {
        return "Terminé. Relancez-le depuis le menu.";
    }
    if (_mode == Mode::Auto) {
        return "Mode auto : les étapes s'enchaînent seules.";
    }
    return "Mode manuel : appuyez sur espace pour l'étape suivante.";
}

/**
 * @brief Applique l'étape courante et s'arrête quand le scénario est fini.
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
