/*
** Axel BATTIGELLI, 2026
** TP1 - Complément en programmation
** File description:
** Construction pas à pas d'un arbre binaire
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
 * @brief Insère des valeurs dans un arbre binaire de recherche, une comparaison à la fois.
 */

namespace
{
    constexpr int kValues[] = {8, 3, 10, 1, 6, 14, 4};
}

/**
 * @brief Construit l'exemple de construction d'arbre avec la police de l'application.
 * @param font Police du panneau et des noeuds.
 */
TreeBuild::TreeBuild(const sf::Font &font) : Example(font) {}

/**
 * @brief Renvoie le nom affiché dans le menu.
 * @return Le libellé de menu de cet exemple.
 */
auto TreeBuild::name() const -> const char *
{
    return "Arbre binaire";
}

/**
 * @brief Vide l'arbre visible et prépare les étapes d'insertion.
 * @param mode Auto ou Manuel.
 */
void TreeBuild::start(Mode mode)
{
    _steps.clear();
    _tree.clear();
    _cursor = nullptr;
    _active = nullptr;
    _message = "L'arbre est vide.";
    buildScenario();
    prepare(mode, _steps.size());
}

/**
 * @brief Enregistre chaque comparaison et insertion pour les valeurs fixes.
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
 * @brief Parcourt un arbre temporaire et mémorise une étape par comparaison.
 * @param simulation Arbre qui contient déjà les valeurs précédentes.
 * @param value Valeur dont le chemin est enregistré.
 */
void TreeBuild::addSteps(BinaryTree &simulation, int value)
{
    if (simulation.empty()) {
        simulation.addRoot(value);
        _steps.push_back(Step{.kind = StepKind::Root,
            .fromRoot = false,
            .left = false,
            .value = value,
            .message = std::to_string(value) + " devient la racine."});
        return;
    }

    bool first = true;
    TreeNode *current = simulation.root();
    while (current != nullptr) {
        if (value < current->value()) {
            if (current->left() == nullptr) {
                simulation.addChild(*current, true, value);
                _steps.push_back(Step{.kind = StepKind::Place,
                    .fromRoot = first,
                    .left = true,
                    .value = value,
                    .message = std::to_string(value) + " < " + std::to_string(current->value())
                        + " : le placer à gauche."});
                return;
            }
            _steps.push_back(Step{.kind = StepKind::Descend,
                .fromRoot = first,
                .left = true,
                .value = current->left()->value(),
                .message = std::to_string(value) + " < " + std::to_string(current->value())
                    + " : descendre vers " + std::to_string(current->left()->value()) + "."});
            current = current->left();
        } else if (value > current->value()) {
            if (current->right() == nullptr) {
                simulation.addChild(*current, false, value);
                _steps.push_back(Step{.kind = StepKind::Place,
                    .fromRoot = first,
                    .left = false,
                    .value = value,
                    .message = std::to_string(value) + " > " + std::to_string(current->value())
                        + " : le placer à droite."});
                return;
            }
            _steps.push_back(Step{.kind = StepKind::Descend,
                .fromRoot = first,
                .left = false,
                .value = current->right()->value(),
                .message = std::to_string(value) + " > " + std::to_string(current->value())
                    + " : descendre vers " + std::to_string(current->right()->value()) + "."});
            current = current->right();
        } else {
            return;
        }
        first = false;
    }
}

/**
 * @brief Rejoue une étape enregistrée sur l'arbre affiché à l'écran.
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
            _cursor = step.left ? _cursor->left() : _cursor->right();
        } else {
            _active = _tree.addChild(*_cursor, step.left, step.value);
        }
    }
    _message = step.message;
}

/**
 * @brief Dessine l'arbre construit jusqu'ici et met en évidence le noeud courant.
 * @param target Surface qui reçoit l'exemple.
 * @param area Partie gauche de la fenêtre.
 */
void TreeBuild::draw(sf::RenderTarget &target, sf::FloatRect area) const
{
    const DrawArea drawn =
        drawPanel(target, area, font(), "Arbre binaire", stepLabel(), _message, hint());
    drawTree(target, font(), drawn.diagram, _tree.root(), _active);
}
