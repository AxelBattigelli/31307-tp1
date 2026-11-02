/*
** Axel BATTIGELLI, 2026
** TP1 - Complément en programmation
** File description:
** Linked list with smart pointers
*/

#include "Examples/LinkedList/LinkedList.hpp"

#include <cstddef>
#include <memory>
#include <utility>
#include <vector>

/**
 * @file LinkedList.cpp
 * @brief Singly linked list owned by std::unique_ptr.
 */

/**
 * @brief Inserts a value at the back of the list.
 * @param value Value stored in the new node.
 */
void LinkedList::enqueue(int value)
{
    auto node = std::make_unique<ListNode>(value);
    if (!_head) {
        _head = std::move(node);
        return;
    }

    // Raw pointer: walk to the last node without taking ownership.
    ListNode *current = _head.get();
    while (current->next) {
        current = current->next.get();
    }
    current->next = std::move(node);
}

/**
 * @brief Inserts a value at the front. The new node becomes the head.
 * @param value Value stored in the new node.
 */
void LinkedList::push(int value)
{
    auto node = std::make_unique<ListNode>(value);
    // The new top takes ownership of the previous stack.
    node->next = std::move(_head);
    _head = std::move(node);
}

/**
 * @brief Removes the front node and lets the next node become the head.
 * @return False when the list is already empty.
 */
auto LinkedList::dequeue() -> bool
{
    if (!_head) {
        return false;
    }

    // The head takes the next node. The old front is destroyed.
    _head = std::move(_head->next);
    return true;
}

/**
 * @brief Removes the front node. Same operation as dequeue().
 * @return False when the list is already empty.
 */
auto LinkedList::pop() -> bool
{
    return dequeue();
}

/**
 * @brief Destroys every node, starting from the head.
 */
void LinkedList::clear()
{
    _head.reset();
}

/**
 * @brief Tells whether the list has no node.
 * @return True when there is no head.
 */
auto LinkedList::empty() const -> bool
{
    return _head == nullptr;
}

/**
 * @brief Counts the nodes from the head to the end.
 * @return The number of nodes.
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
 * @brief Copies the values in list order, head first.
 * @return The values currently stored.
 */
auto LinkedList::values() const -> std::vector<int>
{
    std::vector<int> result;
    for (const ListNode *node = _head.get(); node != nullptr; node = node->next.get()) {
        result.push_back(node->value);
    }
    return result;
}
