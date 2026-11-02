/*
** Axel BATTIGELLI, 2026
** TP1 - Complément en programmation
** File description:
** Binary search tree with smart pointers
*/

#include "Examples/BinaryTree/BinaryTree.hpp"

#include <memory>

/**
 * @file BinaryTree.cpp
 * @brief Binary search tree whose children are owned by std::unique_ptr.
 */

/**
 * @brief Destroys the whole tree from the root.
 */
void BinaryTree::clear()
{
    _root.reset();
}

/**
 * @brief Tells whether the tree has no root.
 * @return True when the tree is empty.
 */
auto BinaryTree::empty() const -> bool
{
    return _root == nullptr;
}

/**
 * @brief Gives a pointer to the root so a caller can walk the tree.
 * @return The root, or nullptr when the tree is empty.
 */
auto BinaryTree::root() -> TreeNode *
{
    return _root.get();
}

/**
 * @brief Gives a read-only pointer to the root.
 * @return The root, or nullptr when the tree is empty.
 */
auto BinaryTree::root() const -> const TreeNode *
{
    return _root.get();
}

/**
 * @brief Creates the root node.
 * @param value Value stored in the root.
 */
void BinaryTree::addRoot(int value)
{
    _root = std::make_unique<TreeNode>(value);
}

/**
 * @brief Attaches a new child to a parent. The parent owns that child.
 * @param parent Node that receives the child.
 * @param left True for the left child, false for the right child.
 * @param value Value stored in the new node.
 * @return The node that was just created.
 */
auto BinaryTree::addChild(TreeNode &parent, bool left, int value) -> TreeNode *
{
    auto &link = left ? parent.left : parent.right;
    link = std::make_unique<TreeNode>(value);
    return link.get();
}

/**
 * @brief Inserts a value by walking left or right until an empty link.
 * @param value Value to insert. A value already present is ignored.
 */
void BinaryTree::insert(int value)
{
    if (!_root) {
        _root = std::make_unique<TreeNode>(value);
        return;
    }

    // Walk with a raw pointer. Only the parent owns the new node.
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
