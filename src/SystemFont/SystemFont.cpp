/*
** Axel BATTIGELLI, 2026
** TP1 - Complément en programmation
** File description:
** Application font loaded once from assets
*/

#include "SystemFont/SystemFont.hpp"

#include <SFML/Graphics/Font.hpp>
#include <array>
#include <filesystem>
#include <iostream>
#include <string>

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#else
#include <unistd.h>
#endif

/**
 * @file SystemFont.cpp
 * @brief Opens assets/OpenSans.ttf once and keeps it for the whole program.
 */

namespace
{

auto executableDirectory() -> std::filesystem::path
{
#ifdef _WIN32
    std::array<char, 4096> buffer{};
    const DWORD length =
        GetModuleFileNameA(nullptr, buffer.data(), static_cast<DWORD>(buffer.size()));
    if (length == 0 || length >= buffer.size()) {
        return {};
    }
#else
    std::array<char, 4096> buffer{};
    const ssize_t length = ::readlink("/proc/self/exe", buffer.data(), buffer.size() - 1);
    if (length <= 0) {
        return {};
    }
    buffer[static_cast<std::size_t>(length)] = '\0';
#endif
    return std::filesystem::path(buffer.data()).parent_path();
}

} // namespace

/**
 * @brief Loads assets/OpenSans.ttf the first time it is called.
 * @return True when the font file was found and opened.
 */
auto SystemFont::load() -> bool
{
    if (_loaded) {
        return true;
    }

    const std::array<std::filesystem::path, 2> candidates = {
        std::filesystem::path("assets") / "OpenSans.ttf",
        executableDirectory() / "assets" / "OpenSans.ttf",
    };
    for (const std::filesystem::path &candidate : candidates) {
        if (_font.loadFromFile(candidate.string())) {
            _loaded = true;
            return true;
        }
    }

    std::cerr << "Could not load assets/OpenSans.ttf.\n";
    return false;
}

/**
 * @brief Gives the font shared by the menu and the examples.
 * @return The font. Valid after load() succeeds.
 */
auto SystemFont::font() -> sf::Font &
{
    return _font;
}

/**
 * @brief Gives the font shared by the menu and the examples.
 * @return The font. Valid after load() succeeds.
 */
auto SystemFont::font() const -> const sf::Font &
{
    return _font;
}
