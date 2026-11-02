/*
** Axel BATTIGELLI, 2026
** TP1 - Complément en programmation
** File description:
** Base class for the four examples
*/

#pragma once

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <cstddef>
#include <string>

class Example {
  public:
    enum class Mode { Auto, Manual };

    explicit Example(const sf::Font &font);
    virtual ~Example() = default;

    Example(const Example &) = delete;
    auto operator=(const Example &) -> Example & = delete;

    [[nodiscard]] virtual auto name() const -> const char * = 0;
    virtual void start(Mode mode) = 0;
    void update(float dt);
    void nextStep();
    virtual void draw(sf::RenderTarget &target, sf::FloatRect area) const = 0;

  protected:
    void prepare(Mode mode, std::size_t total);
    virtual void applyStep() = 0;

    [[nodiscard]] auto font() const -> const sf::Font &;
    [[nodiscard]] auto index() const -> std::size_t;
    [[nodiscard]] auto stepLabel() const -> std::string;
    [[nodiscard]] auto hint() const -> const char *;

  private:
    void runStep();

    const sf::Font &_font;
    Mode _mode = Mode::Manual;
    bool _started = false;
    bool _finished = false;
    std::size_t _index = 0;
    std::size_t _total = 0;
    float _elapsed = 0.F;
};
