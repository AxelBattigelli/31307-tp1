/*
** Axel BATTIGELLI, 2026
** TP1 - Complément en programmation
** File description:
** Linked list declaration
*/

#pragma once

#include <memory>
#include <vector>

struct ListNode {
    int value;
    std::unique_ptr<ListNode> next;

    explicit ListNode(int v) : value(v) {}
};

class LinkedList {
  public:
    void enqueue(int value);
    void push(int value);
    auto dequeue() -> bool;
    auto pop() -> bool;
    void clear();
    [[nodiscard]] auto empty() const -> bool;
    [[nodiscard]] auto size() const -> std::size_t;
    [[nodiscard]] auto values() const -> std::vector<int>;

  private:
    std::unique_ptr<ListNode> _head;
};
