#pragma once

#include "DynamicArray.hpp"
#include "Sequence.hpp"
#include <stdexcept>

template <typename T> class ArraySequence : public Sequence<T> {
protected:
  DynamicArray<T> data;

  virtual ArraySequence<T> *clone() const = 0;
  virtual ArraySequence<T> *createEmpty() const = 0;

public:
  ArraySequence() : data() {}
  ArraySequence(int size) : data(size) {}
  ArraySequence(const T *source, int amount) : data(source, amount) {}
  ArraySequence(const ArraySequence &other) : data(other.data) {}
  ArraySequence &operator=(const ArraySequence &other) {
    data = other.data;
    return *this;
  }

  const T &getFirst() const override {
    if (data.getSize() == 0)
      throw std::out_of_range("ArraySequence::getFirst: empty");
    return data.get(0);
  }

  const T &getLast() const override {
    if (data.getSize() == 0)
      throw std::out_of_range("ArraySequence::getLast: empty");
    return data.get(data.getSize() - 1);
  }

  const T &get(int index) const override { return data.get(index); }

  int getLength() const override { return data.getSize(); }

  Sequence<T> *append(const T &value) override {
    ArraySequence<T> *result = clone();
    int newSize = result->data.getSize() + 1;
    result->data.resize(newSize);
    result->data.set(newSize - 1, value);
    return result;
  }

  Sequence<T> *prepend(const T &value) override {
    ArraySequence<T> *result = clone();
    int oldSize = result->data.getSize();
    result->data.resize(oldSize + 1);
    for (int i = oldSize; i > 0; --i) {
      result->data.set(i, result->data.get(i - 1));
    }
    result->data.set(0, value);
    return result;
  }

  Sequence<T> *insertAt(const T &value, int index) override {
    if (index < 0 || index > data.getSize())
      throw std::out_of_range("ArraySequence::insertAt: index out of range");
    if (index == 0)
      return prepend(value);
    if (index == data.getSize())
      return append(value);

    ArraySequence<T> *result = clone();
    int oldSize = result->data.getSize();
    result->data.resize(oldSize + 1);
    for (int i = oldSize; i > index; --i) {
      result->data.set(i, result->data.get(i - 1));
    }
    result->data.set(index, value);
    return result;
  }

  Sequence<T> *remove(int index) override {
    if (index < 0 || index >= data.getSize())
      throw std::out_of_range("ArraySequence::remove: index out of range");

    ArraySequence<T> *result = clone();
    int oldSize = result->data.getSize();
    for (int i = index; i < oldSize - 1; ++i) {
      result->data.set(i, result->data.get(i + 1));
    }
    result->data.resize(oldSize - 1);
    return result;
  }

  Sequence<T> *getSubsequence(int start, int end) const override {
    if (start < 0 || end >= data.getSize() || start > end)
      throw std::out_of_range("ArraySequence::getSubsequence: invalid indices");
    int size = end - start + 1;
    ArraySequence<T> *result = createEmpty();
    result->data.resize(size);
    for (int i = 0; i < size; ++i) {
      result->data.set(i, data.get(start + i));
    }
    return result;
  }

  Sequence<T> *concat(const Sequence<T> &other) const override {
    int thisSize = data.getSize();
    int otherSize = other.getLength();
    ArraySequence<T> *result = createEmpty();
    result->data.resize(thisSize + otherSize);
    for (int i = 0; i < thisSize; ++i) {
      result->data.set(i, data.get(i));
    }
    for (int i = 0; i < otherSize; ++i) {
      result->data.set(thisSize + i, other.get(i));
    }
    return result;
  }

  virtual ~ArraySequence() override {}
};

template <typename T> class MutableArraySequence : public ArraySequence<T> {
protected:
  ArraySequence<T> *clone() const override {
    return const_cast<MutableArraySequence<T> *>(this);
  }
  ArraySequence<T> *createEmpty() const override {
    return new MutableArraySequence<T>();
  }

public:
  using ArraySequence<T>::ArraySequence;
};

template <typename T> class ImmutableArraySequence : public ArraySequence<T> {
protected:
  ArraySequence<T> *clone() const override {
    return new ImmutableArraySequence<T>(*this);
  }
  ArraySequence<T> *createEmpty() const override {
    return new ImmutableArraySequence<T>();
  }

public:
  using ArraySequence<T>::ArraySequence;
};