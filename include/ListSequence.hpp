#pragma once

#include "LinkedList.hpp"
#include "Sequence.hpp"
#include <stdexcept>

template <typename T> class ListSequence : public Sequence<T> {
protected:
  LinkedList<T> data;

public:
  ListSequence() : data() {}
  ListSequence(const T *source, int amount) : data(source, amount) {}
  ListSequence(const ListSequence &other) : data(other.data) {}

  ListSequence &operator=(const ListSequence &other) {
    data = other.data;
    return *this;
  }

  const T &getFirst() const override { return data.getFirst(); }

  const T &getLast() const override { return data.getLast(); }

  const T &get(int index) const override { return data.get(index); }

  void set(const T &value, int index) override { data.set(value, index); }

  int getSize() const override { return data.getSize(); }

  void append(const T &value) override { data.append(value); }

  void prepend(const T &value) override { data.prepend(value); }

  void insertAt(const T &value, int index) override {
    data.insertAt(value, index);
  }

  void removeAt(int index) override { data.removeAt(index); }

  Sequence<T> *getSubsequence(int start, int end) const override {
    auto *result = new ListSequence<T>();
    result->data = data.getSublist(start, end);
    return result;
  }

  Sequence<T> *concat(const Sequence<T> &other) const override {
    LinkedList<T> temp;
    for (int i = 0; i < other.getSize(); ++i) {
      temp.append(other.get(i));
    }
    auto *result = new ListSequence<T>();
    result->data = data.concat(temp);
    return result;
  }

  virtual ~ListSequence() override {}
};