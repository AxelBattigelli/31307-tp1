/*
** Axel BATTIGELLI, 2026
** TP1 - Complément en programmation
** File description:
** Déclaration de l'arbre binaire de recherche
*/

#pragma once

#include <memory>

class TreeNode {
  public:
    explicit TreeNode(int value) : _value(value) {}

    [[nodiscard]] auto value() const -> int { return _value; }

    [[nodiscard]] auto left() const -> const TreeNode * { return _left.get(); }
    [[nodiscard]] auto right() const -> const TreeNode * { return _right.get(); }
    auto left() -> TreeNode * { return _left.get(); }
    auto right() -> TreeNode * { return _right.get(); }

  private:
    friend class BinaryTree;

    int _value;
    std::unique_ptr<TreeNode> _left;
    std::unique_ptr<TreeNode> _right;
};

class BinaryTree {
  public:
    void clear();
    [[nodiscard]] auto empty() const -> bool;
    auto root() -> TreeNode *;
    [[nodiscard]] auto root() const -> const TreeNode *;

    void addRoot(int value);
    static auto addChild(TreeNode &parent, bool left, int value) -> TreeNode *;
    void insert(int value);

  private:
    std::unique_ptr<TreeNode> _root;
};
