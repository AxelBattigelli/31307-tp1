/*
** Axel BATTIGELLI, 2026
** TP1 - Complément en programmation
** File description:
** Déclaration de l'exemple de construction d'arbre
*/

#pragma once

#include "Examples/BinaryTree/BinaryTree.hpp"
#include "Examples/Example/Example.hpp"

#include <string>
#include <vector>

class TreeBuild : public Example {
  public:
    explicit TreeBuild(const sf::Font &font);

    [[nodiscard]] auto name() const -> const char * override;
    void start(Mode mode) override;
    void draw(sf::RenderTarget &target, sf::FloatRect area) const override;

  private:
    enum class StepKind { Root, Descend, Place };

    struct Step {
        StepKind kind;
        bool fromRoot;
        bool left;
        int value;
        std::string message;
    };

    void buildScenario();
    void addSteps(BinaryTree &simulation, int value);
    void applyStep() override;

    std::vector<Step> _steps;
    BinaryTree _tree;
    TreeNode *_cursor = nullptr;
    const TreeNode *_active = nullptr;
    std::string _message = "Choisissez Auto ou Manuel dans le menu.";
};
