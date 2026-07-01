#pragma once

#include "ArraySequence.hpp"
#include <algorithm>
#include <random>

template <typename T>
void almostUniformPermutation(Sequence<T> *seq, int segments = 4) {
  int amount = seq->getSize();
  if (amount <= 1)
    return;

  if (segments < 2)
    segments = 2;
  if (segments > amount)
    segments = amount;

  std::random_device rd;
  std::mt19937 gen(rd());

  ArraySequence<int> bounds(amount + 1); // хранение границ (0 и n)
  for (int i = 0; i <= amount; ++i) {
    bounds.set((amount * i) / segments, i);
  }

  for (int d = segments / 2; d >= 1; --d) {
    for (int i = 0; i + d < segments; ++i) {
      int seg1Start = bounds.get(i);
      int seg1End = bounds.get(i + 1);
      int seg2Start = bounds.get(i + d);
      int seg2End = bounds.get(i + d + 1);

      int seg1Size = seg1End - seg1Start;
      int seg2Size = seg2End - seg2Start;

      if (seg1Size == 0 || seg2Size == 0)
        continue;

      int swaps = std::min(seg1Size, seg2Size) / 2;
      if (swaps < 1)
        swaps = 1;

      for (int s = 0; s < swaps; ++s) {
        std::uniform_int_distribution<> dist1(seg1Start, seg1End - 1);
        std::uniform_int_distribution<> dist2(seg2Start, seg2End - 1);

        int idx1 = dist1(gen);
        int idx2 = dist2(gen);

        T temp = seq->get(idx1);
        seq->set(seq->get(idx2), idx1);
        seq->set(temp, idx2);
      }
    }
  }
}