#pragma once

#include <algorithm>
#include <stdexcept>

template <typename T> class DynamicArray {
private:
  T *data;
  int size;

public:
  DynamicArray() : data(nullptr), size(0) {}

  DynamicArray(int size) : data(nullptr), size(0) {
    if (size < 0)
      throw std::invalid_argument("DynamicArray: size cannot be negative");
    if (size > 0)
      data = new T[size];
    this->size = size;
  }

  DynamicArray(const T *source, int amount) : data(nullptr), size(0) {
    if (amount < 0)
      throw std::invalid_argument("DynamicArray: amount cannot be negative");
    if (amount > 0 && source == nullptr)
      throw std::invalid_argument("DynamicArray: null pointer received");
    if (amount > 0) {
      data = new T[amount];
      std::copy(source, source + amount, data);
      size = amount;
    }
  }

  DynamicArray(const DynamicArray &other) : data(nullptr), size(0) {
    if (other.size > 0) {
      data = new T[other.size];
      std::copy(other.data, other.data + other.size, data);
      size = other.size;
    }
  }

  DynamicArray &operator=(const DynamicArray &other) {
    if (this != &other) {
      DynamicArray temp(other);
      std::swap(data, temp.data);
      std::swap(size, temp.size);
    }
    return *this;
  }

  ~DynamicArray() { delete[] data; }

  const T &get(int index) const {
    if (index < 0 || index >= size)
      throw std::out_of_range("DynamicArray->get: index out of range");
    return data[index];
  }

  void set(int index, const T &value) {
    if (index < 0 || index >= size)
      throw std::out_of_range("DynamicArray->set: index out of range");
    data[index] = value;
  }

  int getSize() const { return size; }

  void resize(int newSize) {
    if (newSize < 0)
      throw std::invalid_argument(
          "DynamicArray->resize: new size cannot be negative");
    if (newSize == size)
      return;

    T *newData = nullptr;
    if (newSize > 0) {
      newData = new T[newSize];
      std::copy(data, data + std::min(size, newSize), newData);
    }
    delete[] data;
    data = newData;
    size = newSize;
  }
};