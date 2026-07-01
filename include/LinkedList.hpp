#pragma once

#include <stdexcept>
#include <utility>

template <typename T> class LinkedList {
private:
  struct Node {
    T data;
    Node *next;
    Node(const T &value) : data(value), next(nullptr) {}
  };
  Node *head, *tail;
  int size;

public:
  LinkedList() : head(nullptr), tail(nullptr), size(0) {}

  LinkedList(const T *source, int amount)
      : head(nullptr), tail(nullptr), size(0) {
    if (amount < 0)
      throw std::invalid_argument("LinkedList: amount cannot be negative");
    if (amount > 0 && source == nullptr)
      throw std::invalid_argument("LinkedList: null pointer received");
    for (int i = 0; i < amount; ++i) {
      append(source[i]);
    }
  }

  LinkedList(const LinkedList &other) : head(nullptr), tail(nullptr), size(0) {
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
      std::swap(size, temp.size);
    }
    return *this;
  }

  ~LinkedList() {
    Node *temp = nullptr;
    while (head != nullptr) {
      temp = head;
      head = head->next;
      delete temp;
    }
  }

  const T &getFirst() const {
    if (size == 0)
      throw std::out_of_range("LinkedList->getFirst: list is empty");
    return head->data;
  }

  const T &getLast() const {
    if (size == 0)
      throw std::out_of_range("LinkedList->getLast: list is empty");
    return tail->data;
  }

  const T &get(int index) const {
    if (index < 0 || index >= size)
      throw std::out_of_range("LinkedList->get: index out of range");
    Node *current = head;
    for (int i = 0; i < index; ++i) {
      current = current->next;
    }
    return current->data;
  }

  int getSize() const { return size; }

  void set(const T &value, int index) {
    if (index < 0 || index >= size)
      throw std::out_of_range("LinkedList->set: index out of range");
    Node *current = head;
    for (int i = 0; i < index; ++i) {
      current = current->next;
    }
    current->data = value;
  }

  void append(const T &value) {
    Node *newNode = new Node(value);
    if (size == 0) {
      head = tail = newNode;
    } else {
      tail->next = newNode;
      tail = newNode;
    }
    ++size;
  }

  void prepend(const T &value) {
    Node *newNode = new Node(value);
    if (size == 0) {
      head = tail = newNode;
    } else {
      newNode->next = head;
      head = newNode;
    }
    ++size;
  }

  void insertAt(const T &value, int index) {
    if (index < 0 || index > size) {
      throw std::out_of_range("LinkedList->insertAt: index out of range");
    } else if (index == 0) {
      prepend(value);
    } else if (index == size) {
      append(value);
    } else {
      Node *prev = head;
      for (int i = 0; i < index - 1; ++i) {
        prev = prev->next;
      }
      Node *newNode = new Node(value);
      newNode->next = prev->next;
      prev->next = newNode;
      ++size;
    }
    return;
  }

  void removeAt(int index) {
    if (index < 0 || index >= size)
      throw std::out_of_range("LinkedList->remove: index out of range");
    if (size == 1) {
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
      if (index == size - 1) {
        tail = prev;
      }
      delete temp;
    }
    --size;
  }

  LinkedList<T> getSublist(int start, int end) const {
    if (start < 0 || end >= size || start > end)
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
    LinkedList<T> result(*this);
    Node *current = other.head;
    while (current != nullptr) {
      result.append(current->data);
      current = current->next;
    }
    return result;
  }
};
