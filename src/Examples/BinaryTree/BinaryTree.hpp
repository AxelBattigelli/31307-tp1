/*
** Axel BATTIGELLI, 2026
** TP1 - Complément en programmation
** File description:
** Déclaration de l'arbre binaire de recherche
*/

#pragma once

#include <memory>

struct TreeNode {
    int value;
    std::unique_ptr<TreeNode> left;
    std::unique_ptr<TreeNode> right;

    explicit TreeNode(int v) : value(v) {}
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
