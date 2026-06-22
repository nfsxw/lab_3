#pragma once
#include "ArraySequence.hpp"
#include <cstddef>

template <typename T> int countInversions(const Sequence<T> &seq) {
  int result = 0;
  int n = seq.getLength();
  for (int i = 0; i < n; ++i)
    for (int j = i + 1; j < n; ++j)
      if (seq.get(i) > seq.get(j))
        ++result;
  return result;
}

template <typename T>
MutableArraySequence<MutableArraySequence<T>> *
allCombinations(const Sequence<T> &seq) {
  auto result = new MutableArraySequence<MutableArraySequence<T>>();
  result->append(MutableArraySequence<T>());

  for (int i = 0; i < seq.getLength(); ++i) {
    int currentSize = result->getLength();
    for (int j = 0; j < currentSize; ++j) {
      MutableArraySequence<T> newSubset = result->get(j);
      newSubset.append(seq.get(i));
      result->append(newSubset);
    }
  }
  return result;
}

template <typename T>
MutableArraySequence<MutableArraySequence<T>> *
allSubsequences(const Sequence<T> &seq) {
  auto result = new MutableArraySequence<MutableArraySequence<T>>();
  result->append(MutableArraySequence<T>());
  for (int i = 0; i < seq.getLength(); ++i) {
    auto newSubset = MutableArraySequence<T>();
    newSubset.append(seq.get(i));
    result->append(newSubset);
    for (int j = i + 1; j < seq.getLength(); ++j) {
      newSubset.append(seq.get(j));
      result->append(newSubset);
    }
  }
  return result;
}