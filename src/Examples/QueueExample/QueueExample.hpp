/*
** Axel BATTIGELLI, 2026
** TP1 - Complément en programmation
** File description:
** Déclaration de l'exemple de file
*/

#pragma once

#include "Examples/Example/Example.hpp"
#include "Examples/LinkedList/LinkedList.hpp"

#include <string>

class QueueExample : public Example {
  public:
    explicit QueueExample(const sf::Font &font);

    [[nodiscard]] auto name() const -> const char * override;
    void start(Mode mode) override;
    void draw(sf::RenderTarget &target, sf::FloatRect area) const override;

  private:
    void applyStep() override;

    LinkedList _list;
    int _focus = -1;
    std::string _message = "Choisissez Auto ou Manuel dans le menu.";
};
