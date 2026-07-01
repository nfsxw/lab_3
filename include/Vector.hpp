#pragma once

#include "ArraySequence.hpp"
#include <cmath>
#include <complex>
#include <stdexcept>

template <typename T> class Vector {
private:
  ArraySequence<T> data;

public:
  Vector() : data() {};
  Vector(int size) : data(size) {}
  Vector(const T *source, int amount) : data(source, amount) {}
  Vector(const Sequence<T> &other) {
    for (int i = 0; i < other.getSize(); ++i) {
      data.append(other.get(i));
    }
  }

  int getSize() const { return data.getSize(); }

  T get(int index) const { return data.get(index); }

  void set(const T &value, int index) {
    data.set(value, index);
    return;
  }

  Vector<T> add(const Vector<T> &other) const {
    if (getSize() != other.getSize())
      throw std::invalid_argument("Vector sizes differ");
    Vector<T> result(getSize());
    for (int i = 0; i < getSize(); ++i)
      result.set(get(i) + other.get(i), i);
    return result;
  }

  T dotProduct(const Vector<T> &other) const {
    if (getSize() != other.getSize())
      throw std::invalid_argument("Vector sizes differ");
    T sum = T();
    for (int i = 0; i < getSize(); ++i)
      sum += get(i) * other.get(i);
    return sum;
  }

  Vector<T> scalarMul(const T &scalar) const {
    Vector<T> result(getSize());
    for (int i = 0; i < getSize(); ++i)
      result.set(data.get(i) * scalar, i);
    return result;
  }

  double norm() const {
    double sum = 0.0;
    for (int i = 0; i < getSize(); ++i) {
      double val = std::abs(get(i));
      sum += val * val;
    }
    return std::sqrt(sum);
  }
};