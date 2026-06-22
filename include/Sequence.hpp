#pragma once

template <typename T> class Sequence {
public:
  virtual ~Sequence() {}

  virtual const T &getFirst() const = 0;
  virtual const T &getLast() const = 0;
  virtual const T &get(int index) const = 0;
  virtual int getLength() const = 0;

  virtual Sequence<T> *append(const T &item) = 0;
  virtual Sequence<T> *prepend(const T &item) = 0;
  virtual Sequence<T> *insertAt(const T &item, int index) = 0;
  virtual Sequence<T> *remove(int index) = 0;

  virtual Sequence<T> *getSubsequence(int start, int end) const = 0;
  virtual Sequence<T> *concat(const Sequence<T> &other) const = 0;
};