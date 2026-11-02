/*
** Axel BATTIGELLI, 2026
** TP1 - Complément en programmation
** File description:
** Recherche pas à pas dans un arbre binaire
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
 * @brief Parcourt un arbre binaire déjà construit, y compris des valeurs absentes.
 */

namespace
{
    constexpr int kValues[] = {8, 3, 10, 1, 6, 14, 4};
    constexpr int kTargets[] = {6, 14, 2, 9};
} // namespace

/**
 * @brief Construit l'exemple de recherche avec la police de l'application.
 * @param font Police du panneau et des nœuds.
 */
TreeSearch::TreeSearch(const sf::Font &font) : Example(font) {}

/**
 * @brief Renvoie le nom affiché dans le menu.
 * @return Le libellé de menu de cet exemple.
 */
auto TreeSearch::name() const -> const char *
{
    return "Recherche";
}

/**
 * @brief Construit l'arbre, puis enregistre la recherche de chaque cible.
 * @param mode Auto ou Manuel.
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
    _message = "Arbre prêt. Recherche de 6, 14, 2, puis 9.";
    prepare(mode, _steps.size());
}

/**
 * @brief Mémorise une étape pour chaque comparaison faite en cherchant une valeur.
 * @param target Valeur à chercher. Elle peut être absente de l'arbre.
 */
void TreeSearch::record(int target)
{
    const TreeNode *node = _tree.root();
    const std::string targetText = std::to_string(target);
    bool first = true;

    while (node != nullptr) {
        const std::string here = std::to_string(node->value);
        const std::string prefix = first ? "Recherche de " + targetText + ". " : "";
        first = false;

        if (node->value == target) {
            _steps.push_back(
                Step{.node = node, .message = prefix + targetText + " est dans l'arbre."});
            return;
        }

        if (target < node->value) {
            if (node->left == nullptr) {
                _steps.push_back(Step{.node = node,
                    .message = prefix + targetText + " < " + here
                        + " et le fils gauche est vide : introuvable."});
                return;
            }
            _steps.push_back(Step{.node = node,
                .message = prefix + targetText + " < " + here + " : aller à gauche."});
            node = node->left.get();
        } else {
            if (node->right == nullptr) {
                _steps.push_back(Step{.node = node,
                    .message = prefix + targetText + " > " + here
                        + " et le fils droit est vide : introuvable."});
                return;
            }
            _steps.push_back(Step{.node = node,
                .message = prefix + targetText + " > " + here + " : aller à droite."});
            node = node->right.get();
        }
    }
}

/**
 * @brief Met en évidence le nœud comparé pendant l'étape de recherche courante.
 */
void TreeSearch::applyStep()
{
    const Step &step = _steps[index()];
    _active = step.node;
    _message = step.message;
}

/**
 * @brief Dessine l'arbre complet et le nœud de la comparaison courante.
 * @param target Surface qui reçoit l'exemple.
 * @param area Partie gauche de la fenêtre.
 */
void TreeSearch::draw(sf::RenderTarget &target, sf::FloatRect area) const
{
    const DrawArea drawn =
        drawPanel(target, area, font(), "Recherche", stepLabel(), _message, hint());
    drawTree(target, font(), drawn.diagram, _tree.root(), _active);
}
