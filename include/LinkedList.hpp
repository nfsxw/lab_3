#pragma once

#include <algorithm>
#include <stdexcept>

template <typename T> class LinkedList {
private:
  struct Node {
    T data;
    Node *next;
    Node(const T &value) : data(value), next(nullptr) {}
  };
  Node *head, *tail;
  int length;

public:
  LinkedList() : head(nullptr), tail(nullptr), length(0) {}

  LinkedList(const T *source, int amount)
      : head(nullptr), tail(nullptr), length(0) {
    if (amount < 0)
      throw std::invalid_argument("LinkedList: count cannot be negative");
    if (amount > 0 && source == nullptr)
      throw std::invalid_argument("LinkedList: null pointer received");
    for (int i = 0; i < amount; ++i) {
      append(source[i]);
    }
  }

  LinkedList(const LinkedList &other)
      : head(nullptr), tail(nullptr), length(0) {
    Node *current = other.head;
    while (current != nullptr) {
      append(current->data);
      current = current->next;
    }
  }

  LinkedList &operator=(const LinkedList &other) {
    if (this != &other) {
      LinkedList temp(other);
      std::swap(head, temp.head);
      std::swap(tail, temp.tail);
      std::swap(length, temp.length);
    }
    return *this;
  }

  ~LinkedList() {
    Node *current = head;
    while (current != nullptr) {
      Node *next = current->next;
      delete current;
      current = next;
    }
  }

  const T &getFirst() const {
    if (length == 0)
      throw std::out_of_range("LinkedList->getFirst: list is empty");
    return head->data;
  }

  const T &getLast() const {
    if (length == 0)
      throw std::out_of_range("LinkedList->getLast: list is empty");
    return tail->data;
  }

  const T &get(int index) const {
    if (index < 0 || index >= length)
      throw std::out_of_range("LinkedList->get: index out of range");
    Node *current = head;
    for (int i = 0; i < index; ++i) {
      current = current->next;
    }
    return current->data;
  }

  int getLength() const { return length; }

  void append(const T &value) {
    Node *newNode = new Node(value);
    if (length == 0) {
      head = tail = newNode;
    } else {
      tail->next = newNode;
      tail = newNode;
    }
    ++length;
  }

  void prepend(const T &value) {
    Node *newNode = new Node(value);
    if (length == 0) {
      head = tail = newNode;
    } else {
      newNode->next = head;
      head = newNode;
    }
    ++length;
  }

  void insertAt(const T &value, int index) {
    if (index < 0 || index > length)
      throw std::out_of_range("LinkedList->insertAt: index out of range");

    if (index == 0) {
      prepend(value);
      return;
    }
    if (index == length) {
      append(value);
      return;
    }

    Node *prev = head;
    for (int i = 0; i < index - 1; ++i) {
      prev = prev->next;
    }
    Node *newNode = new Node(value);
    newNode->next = prev->next;
    prev->next = newNode;
    ++length;
  }

  void removeAt(int index) {
    if (index < 0 || index >= length)
      throw std::out_of_range("LinkedList->remove: index out of range");
    if (length == 1) {
      delete head;
      head = tail = nullptr;
    } else if (index == 0) {
      Node *temp = head;
      head = head->next;
      delete temp;
    } else {
      Node *prev = head;
      for (int i = 0; i < index - 1; ++i) {
        prev = prev->next;
      }
      Node *temp = prev->next;
      prev->next = temp->next;
      if (index == length - 1) {
        tail = prev;
      }
      delete temp;
    }
    --length;
  }

  LinkedList<T> getSublist(int start, int end) const {
    if (start < 0 || end >= length || start > end)
      throw std::out_of_range(
          "LinkedList->getSublist: invalid start/end indices");
    LinkedList<T> result;
    Node *current = head;
    for (int i = 0; i < start; ++i) {
      current = current->next;
    }
    for (int i = start; i <= end; ++i) {
      result.append(current->data);
      current = current->next;
    }
    return result;
  }

  LinkedList<T> concat(const LinkedList<T> &other) const {
    LinkedList<T> result;
    Node *current = head;
    while (current != nullptr) {
      result.append(current->data);
      current = current->next;
    }
    current = other.head;
    while (current != nullptr) {
      result.append(current->data);
      current = current->next;
    }
    return result;
  }
};
