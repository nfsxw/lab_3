#pragma once

#include "DynamicArray.hpp"
#include "Sequence.hpp"
#include <stdexcept>

template <typename T> class ArraySequence : public Sequence<T> {
private:
  DynamicArray<T> data;

public:
  ArraySequence() : data() {}
  ArraySequence(int size) : data(size) {}
  ArraySequence(const T *source, int amount) : data(source, amount) {}
  ArraySequence(const ArraySequence &other) : data(other.data) {}

  ArraySequence &operator=(const ArraySequence &other) {
    data = other.data;
    return *this;
  }

  void set(const T &value, int index) override { data.set(value, index); }
  const T &get(int index) const override { return data.get(index); }
  int getSize() const override { return data.getSize(); }

  const T &getFirst() const override {
    if (getSize() == 0)
      throw std::out_of_range("ArraySequence->getFirst: sequence empty");
    return get(0);
  }

  const T &getLast() const override {
    if (getSize() == 0)
      throw std::out_of_range("ArraySequence->getLast: sequence is empty");
    return get(getSize() - 1);
  }

  void append(const T &value) override {
    int oldSize = getSize();
    data.resize(oldSize + 1);
    set(value, oldSize);
    return;
  }

  void prepend(const T &value) override {
    int oldSize = getSize();
    data.resize(oldSize + 1);
    for (int i = oldSize; i > 0; --i) {
      set(get(i - 1), i);
    }
    set(value, 0);
    return;
  }

  void insertAt(const T &value, int index) override {
    int oldSize = getSize();
    if (index < 0 || index > oldSize)
      throw std::out_of_range("ArraySequence->insertAt: index out of range");
    if (index == 0) {
      prepend(value);
    } else if (index == oldSize) {
      append(value);
    } else {
      data.resize(oldSize + 1);
      for (int i = oldSize; i > index; --i) {
        set(get(i - 1), i);
      }
      set(value, index);
    }
    return;
  }

  void removeAt(int index) override {
    int oldSize = getSize();
    if (index < 0 || index >= oldSize)
      throw std::out_of_range("ArraySequence->remove: index out of range");
    if (index != oldSize - 1) {
      for (int i = index; i < oldSize - 1; ++i) {
        set(get(i + 1), i);
      }
    }
    data.resize(oldSize - 1);
    return;
  }

  Sequence<T> *getSubsequence(int start, int end) const override {
    if (start < 0 || end >= getSize() || start > end)
      throw std::out_of_range("ArraySequence->getSubsequence: invalid indices");
    int newSize = end - start + 1;
    auto *result = new ArraySequence<T>(newSize);
    for (int i = 0; i < newSize; ++i) {
      result->set(get(start + i), i);
    }
    return result;
  }

  Sequence<T> *concat(const Sequence<T> &other) const override {
    int thisSize = getSize();
    int otherSize = other.getSize();
    auto *result = new ArraySequence<T>(thisSize + otherSize);

    for (int i = 0; i < thisSize; ++i) {
      result->set(get(i), i);
    }
    for (int i = 0; i < otherSize; ++i) {
      result->set(other.get(i), thisSize + i);
    }
    return result;
  }

  ~ArraySequence() {}
};