/*
** Axel BATTIGELLI, 2026
** TP1 - Complément en programmation
** File description:
** Liste chaînée avec pointeurs intelligents
*/

#include "Examples/LinkedList/LinkedList.hpp"

#include <cstddef>
#include <memory>
#include <utility>
#include <vector>

/**
 * @file LinkedList.cpp
 * @brief Liste simplement chaînée possédée par std::unique_ptr.
 */

/**
 * @brief Insère une valeur en fin de liste.
 * @param value Valeur stockée dans le nouveau nœud.
 */
void LinkedList::enqueue(int value)
{
    auto node = std::make_unique<ListNode>(value);
    if (!_head) {
        _head = std::move(node);
        return;
    }

    // Pointeur brut : parcourt jusqu'au dernier nœud sans en prendre possession.
    ListNode *current = _head.get();
    while (current->next) {
        current = current->next.get();
    }
    current->next = std::move(node);
}

/**
 * @brief Insère une valeur en tête. Le nouveau nœud devient la tête.
 * @param value Valeur stockée dans le nouveau nœud.
 */
void LinkedList::push(int value)
{
    auto node = std::make_unique<ListNode>(value);
    // Le nouveau sommet prend possession de la pile précédente.
    node->next = std::move(_head);
    _head = std::move(node);
}

/**
 * @brief Retire le nœud de tête et laisse le suivant devenir la tête.
 * @return Faux si la liste est déjà vide.
 */
auto LinkedList::dequeue() -> bool
{
    if (!_head) {
        return false;
    }

    // La tête prend le nœud suivant. L'ancienne tête est détruite.
    _head = std::move(_head->next);
    return true;
}

/**
 * @brief Retire le nœud de tête. Même opération que dequeue().
 * @return Faux si la liste est déjà vide.
 */
auto LinkedList::pop() -> bool
{
    return dequeue();
}

/**
 * @brief Détruit chaque nœud, en partant de la tête.
 */
void LinkedList::clear()
{
    _head.reset();
}

/**
 * @brief Indique si la liste n'a aucun nœud.
 * @return Vrai s'il n'y a pas de tête.
 */
auto LinkedList::empty() const -> bool
{
    return _head == nullptr;
}

/**
 * @brief Compte les nœuds de la tête jusqu'à la fin.
 * @return Le nombre de nœuds.
 */
auto LinkedList::size() const -> std::size_t
{
    std::size_t count = 0;
    for (const ListNode *node = _head.get(); node != nullptr; node = node->next.get()) {
        ++count;
    }
    return count;
}

/**
 * @brief Copie les valeurs dans l'ordre de la liste, tête d'abord.
 * @return Les valeurs actuellement stockées.
 */
auto LinkedList::values() const -> std::vector<int>
{
    std::vector<int> result;
    for (const ListNode *node = _head.get(); node != nullptr; node = node->next.get()) {
        result.push_back(node->value);
    }
    return result;
}
