#pragma once

#include "LinkedList.hpp"
#include "Sequence.hpp"
#include <stdexcept>

template <typename T> class ListSequence : public Sequence<T> {
protected:
  LinkedList<T> data;

  virtual ListSequence<T> *clone() const = 0;
  virtual ListSequence<T> *createEmpty() const = 0;

public:
  ListSequence() : data() {}
  ListSequence(const T *source, int amount) : data(source, amount) {}
  ListSequence(const ListSequence &other) : data(other.data) {}

  const T &getFirst() const override { return data.getFirst(); }

  const T &getLast() const override { return data.getLast(); }

  const T &get(int index) const override { return data.get(index); }

  int getLength() const override { return data.getLength(); }

  Sequence<T> *append(const T &value) override {
    ListSequence<T> *result = clone();
    result->data.append(value);
    return result;
  }

  Sequence<T> *prepend(const T &value) override {
    ListSequence<T> *result = clone();
    result->data.prepend(value);
    return result;
  }

  Sequence<T> *insertAt(const T &value, int index) override {
    ListSequence<T> *result = clone();
    result->data.insertAt(value, index);
    return result;
  }

  Sequence<T> *remove(int index) override {
    ListSequence<T> *result = clone();
    result->data.removeAt(index);
    return result;
  }

  Sequence<T> *getSubsequence(int start, int end) const override {
    LinkedList<T> sublist = data.getSublist(start, end);
    ListSequence<T> *result = createEmpty();
    result->data = sublist;
    return result;
  }

  Sequence<T> *concat(const Sequence<T> &other) const override {
    ListSequence<T> *result = createEmpty();
    for (int i = 0; i < data.getLength(); ++i) {
      result->data.append(data.get(i));
    }
    for (int i = 0; i < other.getLength(); ++i) {
      result->data.append(other.get(i));
    }
    return result;
  }

  virtual ~ListSequence() override {}
};

template <typename T> class MutableListSequence : public ListSequence<T> {
protected:
  ListSequence<T> *clone() const override {
    return const_cast<MutableListSequence<T> *>(this);
  }
  ListSequence<T> *createEmpty() const override {
    return new MutableListSequence<T>();
  }

public:
  using ListSequence<T>::ListSequence;
};

template <typename T> class ImmutableListSequence : public ListSequence<T> {
protected:
  ListSequence<T> *clone() const override {
    return new ImmutableListSequence<T>(*this);
  }
  ListSequence<T> *createEmpty() const override {
    return new ImmutableListSequence<T>();
  }

public:
  using ListSequence<T>::ListSequence;
};