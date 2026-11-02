/*
** Axel BATTIGELLI, 2026
** TP1 - Complément en programmation
** File description:
** Déclaration de l'exemple de recherche dans un arbre
*/

#pragma once

#include "Examples/BinaryTree/BinaryTree.hpp"
#include "Examples/Example/Example.hpp"

#include <string>
#include <vector>

class TreeSearch : public Example {
  public:
    explicit TreeSearch(const sf::Font &font);

    [[nodiscard]] auto name() const -> const char * override;
    void start(Mode mode) override;
    void draw(sf::RenderTarget &target, sf::FloatRect area) const override;

  private:
    struct Step {
        const TreeNode *node;
        std::string message;
    };

    void record(int target);
    void applyStep() override;

    std::vector<Step> _steps;
    BinaryTree _tree;
    const TreeNode *_active = nullptr;
    std::string _message = "Choisissez Auto ou Manuel dans le menu.";
};
