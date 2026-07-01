#pragma once

template <typename T> class Sequence {
public:
  virtual ~Sequence() {}

  virtual const T &getFirst() const = 0;
  virtual const T &getLast() const = 0;
  virtual const T &get(int index) const = 0;
  virtual int getSize() const = 0;

  virtual void set(const T &value, int index) = 0;
  virtual void append(const T &value) = 0;
  virtual void prepend(const T &value) = 0;
  virtual void insertAt(const T &value, int index) = 0;
  virtual void removeAt(int index) = 0;

  virtual Sequence<T> *getSubsequence(int start, int end) const = 0;
  virtual Sequence<T> *concat(const Sequence<T> &other) const = 0;
};