/*
** Axel BATTIGELLI, 2026
** TP1 - Complément en programmation
** File description:
** Arbre binaire de recherche avec pointeurs intelligents
*/

#include "Examples/BinaryTree/BinaryTree.hpp"

#include <memory>

/**
 * @file BinaryTree.cpp
 * @brief Arbre binaire de recherche dont les fils sont possédés par std::unique_ptr.
 */

/**
 * @brief Détruit tout l'arbre à partir de la racine.
 */
void BinaryTree::clear()
{
    _root.reset();
}

/**
 * @brief Indique si l'arbre n'a pas de racine.
 * @return Vrai si l'arbre est vide.
 */
auto BinaryTree::empty() const -> bool
{
    return _root == nullptr;
}

/**
 * @brief Donne un pointeur vers la racine pour qu'un appelant puisse parcourir l'arbre.
 * @return La racine, ou nullptr si l'arbre est vide.
 */
auto BinaryTree::root() -> TreeNode *
{
    return _root.get();
}

/**
 * @brief Donne un pointeur en lecture seule vers la racine.
 * @return La racine, ou nullptr si l'arbre est vide.
 */
auto BinaryTree::root() const -> const TreeNode *
{
    return _root.get();
}

/**
 * @brief Crée le nœud racine.
 * @param value Valeur stockée dans la racine.
 */
void BinaryTree::addRoot(int value)
{
    _root = std::make_unique<TreeNode>(value);
}

/**
 * @brief Attache un nouveau fils à un parent. Le parent possède ce fils.
 * @param parent Nœud qui reçoit le fils.
 * @param left Vrai pour le fils gauche, faux pour le fils droit.
 * @param value Valeur stockée dans le nouveau nœud.
 * @return Le nœud qui vient d'être créé.
 */
auto BinaryTree::addChild(TreeNode &parent, bool left, int value) -> TreeNode *
{
    auto &link = left ? parent.left : parent.right;
    link = std::make_unique<TreeNode>(value);
    return link.get();
}

/**
 * @brief Insère une valeur en allant à gauche ou à droite jusqu'à un lien vide.
 * @param value Valeur à insérer. Une valeur déjà présente est ignorée.
 */
void BinaryTree::insert(int value)
{
    if (!_root) {
        _root = std::make_unique<TreeNode>(value);
        return;
    }

    // Parcours avec un pointeur brut. Seul le parent possède le nouveau nœud.
    TreeNode *current = _root.get();
    while (true) {
        if (value < current->value) {
            if (!current->left) {
                current->left = std::make_unique<TreeNode>(value);
                return;
            }
            current = current->left.get();
        } else if (value > current->value) {
            if (!current->right) {
                current->right = std::make_unique<TreeNode>(value);
                return;
            }
            current = current->right.get();
        } else {
            return;
        }
    }
}
