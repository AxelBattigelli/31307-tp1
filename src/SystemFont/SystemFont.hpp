/*
** Axel BATTIGELLI, 2026
** TP1 - Complément en programmation
** File description:
** Application font loaded once from assets
*/

#pragma once

#include <SFML/Graphics/Font.hpp>

class SystemFont {
  public:
    SystemFont() = default;
    ~SystemFont() = default;

    SystemFont(const SystemFont &) = delete;
    auto operator=(const SystemFont &) -> SystemFont & = delete;
    SystemFont(SystemFont &&) = delete;
    auto operator=(SystemFont &&) -> SystemFont & = delete;

    auto load() -> bool;
    auto font() -> sf::Font &;
    auto font() const -> const sf::Font &;

  private:
    sf::Font _font;
    bool _loaded = false;
};
