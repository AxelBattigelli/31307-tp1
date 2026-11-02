/*
** Axel BATTIGELLI, 2026
** TP1 - Complément en programmation
** File description:
** Fonctions de dessin des exemples
*/

#include "Examples/Display/Display.hpp"

#include "Examples/BinaryTree/BinaryTree.hpp"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/System/Vector2.hpp>
#include <algorithm>
#include <array>
#include <map>
#include <string>
#include <utility>

/**
 * @file Display.cpp
 * @brief Panneau, flèches et dessin d'arbre binaire partagés par les exemples.
 */

namespace
{

    const sf::Color kInk(28, 34, 46);
    const sf::Color kMuted(96, 108, 128);
    const sf::Color kActive(70, 128, 196);

    /**
     * @brief Réduit un texte jusqu'à ce qu'il tienne dans une largeur maximale.
     * @param text Texte dont la taille de caractère peut diminuer.
     * @param maxWidth Largeur que le texte doit respecter.
     * @param minimum Plus petite taille de caractère autorisée.
     */
    void shrinkToFit(sf::Text &text, float maxWidth, unsigned minimum)
    {
        while (text.getCharacterSize() > minimum && text.getLocalBounds().width > maxWidth) {
            text.setCharacterSize(text.getCharacterSize() - 1);
        }
    }

    /**
     * @brief Donne à chaque noeud un rang en ordre infixe et note la profondeur de l'arbre.
     * @param node Sous-arbre à indexer. Rien n'est fait pour un noeud nul.
     * @param depth Profondeur de ce noeud. La racine vaut zéro.
     * @param rank Prochain emplacement horizontal libre. Il augmente à chaque noeud visité.
     * @param maxDepth Plus grande profondeur vue jusqu'ici.
     */
    void indexNodes(const TreeNode *node, int depth, int &rank, int &maxDepth,
        std::map<const TreeNode *, std::pair<int, int>> &info)
    {
        if (node == nullptr) {
            return;
        }

        indexNodes(node->left(), depth + 1, rank, maxDepth, info);
        info[node] = {rank, depth};
        ++rank;
        maxDepth = std::max(depth, maxDepth);
        indexNodes(node->right(), depth + 1, rank, maxDepth, info);
    }

    /**
     * @brief Dessine les traits qui relient un noeud à ses fils.
     * @param target Surface qui reçoit les traits.
     * @param node Sous-arbre dont les liens sont dessinés.
     */
    void drawLinks(sf::RenderTarget &target, const TreeNode *node,
        const std::map<const TreeNode *, std::pair<int, int>> &info, float x0, float y0,
        float stepX, float stepY)
    {
        if (node == nullptr) {
            return;
        }

        const auto &nodeInfo = info.at(node);
        const sf::Vector2f parent(x0 + (nodeInfo.first * stepX), y0 + (nodeInfo.second * stepY));
        const std::array<const TreeNode *, 2> children = {node->left(), node->right()};
        for (const TreeNode *child : children) {
            if (child == nullptr) {
                continue;
            }
            const auto &childInfo = info.at(child);
            const sf::Vector2f end(
                x0 + (childInfo.first * stepX), y0 + (childInfo.second * stepY));
            const std::array<sf::Vertex, 2> line = {
                sf::Vertex(parent, kMuted),
                sf::Vertex(end, kMuted),
            };
            target.draw(line.data(), line.size(), sf::Lines);
            drawLinks(target, child, info, x0, y0, stepX, stepY);
        }
    }

    /**
     * @brief Dessine chaque noeud comme un cercle. Le noeud actif est rempli en bleu.
     * @param target Surface qui reçoit les cercles.
     * @param font Police des valeurs.
     * @param node Sous-arbre à dessiner.
     * @param active Noeud mis en évidence pour l'étape courante. Peut être nul.
     */
    void drawNodes(sf::RenderTarget &target, const sf::Font &font, const TreeNode *node,
        const TreeNode *active, const std::map<const TreeNode *, std::pair<int, int>> &info,
        float x0, float y0, float stepX, float stepY, float radius, sf::FloatRect area)
    {
        if (node == nullptr) {
            return;
        }

        drawNodes(target, font, node->left(), active, info, x0, y0, stepX, stepY, radius, area);
        drawNodes(target, font, node->right(), active, info, x0, y0, stepX, stepY, radius, area);

        const auto &nodeInfo = info.at(node);
        const sf::Vector2f center(x0 + (nodeInfo.first * stepX), y0 + (nodeInfo.second * stepY));
        const bool isActive = node == active;

        sf::CircleShape circle(radius);
        circle.setOrigin(radius, radius);
        circle.setPosition(center);
        circle.setFillColor(isActive ? kActive : sf::Color(244, 246, 250));
        circle.setOutlineThickness(2.F);
        circle.setOutlineColor(isActive ? kActive : sf::Color(180, 188, 202));
        target.draw(circle);

        sf::Text value(utf8(std::to_string(node->value())), font, 16);
        value.setFillColor(isActive ? sf::Color::White : kInk);
        placeText(value, center.x, center.y, true);
        target.draw(value);

        if (nodeInfo.second == 0 && center.y - radius > area.top + 16.F) {
            sf::Text label(utf8("racine"), font, 14);
            label.setFillColor(kMuted);
            placeText(label, center.x, center.y - radius - 12.F, true);
            target.draw(label);
        }
    }

} // namespace

/**
 * @brief Déplace un texte pour qu'un point soit son coin haut-gauche ou son centre.
 * @param text Texte à positionner. Son origine est mise à jour.
 * @param x Position horizontale.
 * @param y Position verticale.
 * @param center Vrai pour centrer le texte sur le point.
 */
void placeText(sf::Text &text, float x, float y, bool center)
{
    const sf::FloatRect bounds = text.getLocalBounds();
    if (center) {
        text.setOrigin(bounds.left + (bounds.width / 2.F), bounds.top + (bounds.height / 2.F));
    } else {
        text.setOrigin(bounds.left, bounds.top);
    }
    text.setPosition(x, y);
}

/**
 * @brief Dessine une flèche horizontale vers la droite.
 * @param target Surface qui reçoit la flèche.
 * @param x1 Début de la hampe.
 * @param x2 Pointe de la flèche.
 * @param y Centre vertical de la flèche.
 */
void drawRightArrow(sf::RenderTarget &target, float x1, float x2, float y)
{
    const float head = 11.F;
    sf::RectangleShape shaft({std::max(0.F, x2 - head - x1), 3.F});
    shaft.setPosition(x1, y - 1.5F);
    shaft.setFillColor(kMuted);
    target.draw(shaft);

    sf::ConvexShape tip(3);
    tip.setPoint(0, {x2, y});
    tip.setPoint(1, {x2 - head, y - 6.F});
    tip.setPoint(2, {x2 - head, y + 6.F});
    tip.setFillColor(kMuted);
    target.draw(tip);
}

/**
 * @brief Dessine une flèche verticale vers le bas.
 * @param target Surface qui reçoit la flèche.
 * @param x Centre horizontal de la flèche.
 * @param y1 Début de la hampe.
 * @param y2 Pointe de la flèche.
 */
void drawDownArrow(sf::RenderTarget &target, float x, float y1, float y2)
{
    const float head = 11.F;
    sf::RectangleShape shaft({3.F, std::max(0.F, y2 - head - y1)});
    shaft.setPosition(x - 1.5F, y1);
    shaft.setFillColor(kMuted);
    target.draw(shaft);

    sf::ConvexShape tip(3);
    tip.setPoint(0, {x, y2});
    tip.setPoint(1, {x - 6.F, y2 - head});
    tip.setPoint(2, {x + 6.F, y2 - head});
    tip.setFillColor(kMuted);
    target.draw(tip);
}

/**
 * @brief Dessine la carte blanche, le titre, le message et l'indication.
 * @param target Surface qui reçoit la carte.
 * @param area Partie gauche de la fenêtre réservée à l'exemple.
 * @param font Police de tous les textes de la carte.
 * @param title Titre en haut de la carte.
 * @param step Compteur d'étapes. Une chaîne vide le masque.
 * @param message Phrase qui explique l'étape courante.
 * @param hint Phrase affichée en bas de la carte.
 * @return Le rectangle de la carte et la zone libre laissée au schéma.
 */
auto drawPanel(sf::RenderTarget &target, sf::FloatRect area, const sf::Font &font,
    const std::string &title, const std::string &step, const std::string &message,
    const std::string &hint) -> DrawArea
{
    DrawArea result;
    if (area.width < 80.F || area.height < 80.F) {
        return result;
    }

    const float cardX = area.left + 24.F;
    const float cardY = area.top + 24.F;
    sf::RectangleShape card({area.width - 48.F, area.height - 48.F});
    card.setPosition(cardX, cardY);
    card.setFillColor(sf::Color::White);
    card.setOutlineThickness(1.F);
    card.setOutlineColor(sf::Color(210, 216, 224));
    target.draw(card);

    const float left = cardX + 28.F;
    const float textWidth = std::max(40.F, card.getSize().x - 56.F);
    float y = cardY + 20.F;

    sf::Text titleText(utf8(title), font, 24);
    titleText.setStyle(sf::Text::Bold);
    titleText.setFillColor(kInk);
    shrinkToFit(titleText, textWidth, 16);
    placeText(titleText, left, y, false);
    target.draw(titleText);
    y += titleText.getCharacterSize() + 10.F;

    if (!step.empty()) {
        sf::Text stepText(utf8(step), font, 16);
        stepText.setFillColor(kMuted);
        shrinkToFit(stepText, textWidth, 12);
        placeText(stepText, left, y, false);
        target.draw(stepText);
        y += stepText.getCharacterSize() + 8.F;
    }

    if (!message.empty()) {
        sf::Text messageText(utf8(message), font, 20);
        messageText.setFillColor(sf::Color(40, 48, 64));
        shrinkToFit(messageText, textWidth, 13);
        placeText(messageText, left, y, false);
        target.draw(messageText);
        y += messageText.getCharacterSize() + 16.F;
    }

    sf::Text hintText(utf8(hint), font, 16);
    hintText.setFillColor(kMuted);
    shrinkToFit(hintText, textWidth, 12);
    const float hintY = cardY + card.getSize().y - 24.F - hintText.getCharacterSize();
    placeText(hintText, left, hintY, false);
    target.draw(hintText);

    result.card = sf::FloatRect(cardX, cardY, card.getSize().x, card.getSize().y);
    result.diagram =
        sf::FloatRect(cardX + 16.F, y, card.getSize().x - 32.F, std::max(0.F, hintY - 16.F - y));
    return result;
}

/**
 * @brief Dispose l'arbre en ordre et dessine ses liens et ses noeuds.
 * @param target Surface qui reçoit l'arbre.
 * @param font Police des valeurs des noeuds.
 * @param area Zone libre à l'intérieur de la carte.
 * @param root Racine de l'arbre. Un arbre vide affiche un court message.
 * @param active Noeud mis en évidence pour l'étape courante. Peut être nul.
 */
void drawTree(sf::RenderTarget &target, const sf::Font &font, sf::FloatRect area,
    const TreeNode *root, const TreeNode *active)
{
    if (area.width < 40.F || area.height < 40.F) {
        return;
    }

    if (root == nullptr) {
        sf::Text empty(utf8("Aucun noeud"), font, 18);
        empty.setFillColor(sf::Color(140, 148, 162));
        placeText(empty, area.left + (area.width / 2.F), area.top + (area.height / 2.F), true);
        target.draw(empty);
        return;
    }

    std::map<const TreeNode *, std::pair<int, int>> info;
    int rank = 0;
    int maxDepth = 0;
    indexNodes(root, 0, rank, maxDepth, info);

    const int count = rank;
    const float radius = 22.F;
    const float stepX = count <= 1
        ? 0.F
        : std::min(84.F, (area.width - radius * 2.F) / static_cast<float>(count - 1));
    const float stepY = maxDepth == 0
        ? 0.F
        : std::min(92.F, (area.height - radius * 2.F - 20.F) / static_cast<float>(maxDepth));
    const float totalW = (count - 1) * stepX;
    const float totalH = maxDepth * stepY;
    const float x0 = area.left + ((area.width - totalW) / 2.F);
    const float y0 = area.top + ((area.height - totalH) / 2.F);

    drawLinks(target, root, info, x0, y0, stepX, stepY);
    drawNodes(target, font, root, active, info, x0, y0, stepX, stepY, radius, area);
}
