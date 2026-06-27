#include <complex>
#include <iomanip>
#include <iostream>
#include <string>

#include "ArraySequence.hpp"
#include "DynamicArray.hpp"
#include "ExtraTasks.hpp"
#include "LinkedList.hpp"
#include "ListSequence.hpp"
#include "Vector.hpp"
#include "QuickSort.hpp"

int testsPassed = 0;
int testsFailed = 0;

namespace std {
string to_string(const complex<double> &c) {
  return "(" + to_string(c.real()) + "," + to_string(c.imag()) + ")";
}
} // namespace std

void printTestResult(const std::string &testName, bool passed) {
  const int width = 80;
  std::cout << std::left << " - " << std::setw(width) << testName << " [";
  if (passed) {
    std::cout << "PASSED";
  } else {
    std::cout << "FAILED";
  }
  std::cout << "]" << std::endl;
}

template <typename T>

void assertEqual(const T &actual, const T &expected,
                 const std::string &testName) {
  if (actual == expected) {
    printTestResult(testName, true);
    ++testsPassed;
  } else {
    printTestResult(testName + " (expected: " + std::to_string(expected) +
                        ", actual: " + std::to_string(actual) + ")",
                    false);
    ++testsFailed;
  }
}

namespace DynamicArrayTests {
void testDefaultConstructor() {
  DynamicArray<int> arr;
  assertEqual(arr.getSize(), 0, "Default constructor creates empty array");
}

void testSizeConstructor() {

  // Positive size:

  DynamicArray<int> arrPos(5);
  assertEqual(arrPos.getSize(), 5,
              "Size constructor creates array of positive size");

  // Zero size:

  DynamicArray<int> arrNull(0);
  assertEqual(arrNull.getSize(), 0,
              "Size constructor creates array of null size");

  // Negative size:

  try {
    DynamicArray<int> arrNeg(-5);
    printTestResult("Size constructor negative size did not throw", false);
    ++testsFailed;
  } catch (const std::invalid_argument &) {
    printTestResult("Size constructor negative size throws invalid_argument",
                    true);
    ++testsPassed;
  } catch (...) {
    printTestResult("Size constructor negative size threw wrong exception",
                    false);
    ++testsFailed;
  }
}

void testConstructorFromArray() {

  int source[] = {1, 2, 3, 4, 5};

  // Positive amount:

  DynamicArray<int> arrPos(source, 5);
  assertEqual(arrPos.getSize(), 5, "Constructor from array with positive size");
  assertEqual(arrPos.get(0), 1,
              "Constructor from array with positive size: first element");
  assertEqual(arrPos.get(4), 5,
              "Constructor from array with positive size: last element");

  // Negative amount:

  try {
    DynamicArray<int> arrNeg(source, -5);
    printTestResult("Constructor from array with negative size did not throw",
                    false);
    ++testsFailed;
  } catch (const std::invalid_argument &) {
    printTestResult(
        "Constructor from array with negative size throws exception", true);
    ++testsPassed;
  } catch (...) {
    printTestResult(
        "Constructor from array with negative size threw wrong exception",
        false);
    ++testsFailed;
  }

  // Zero amount:

  DynamicArray<int> arrZero(source, 0);
  assertEqual(arrZero.getSize(), 0, "Constructor from array with zero size");

  // NullPtr:

  try {
    DynamicArray<int> arrNullPtr(nullptr, 5);
    printTestResult("Constructor from array with null pointer did not throw",
                    false);
    ++testsFailed;
  } catch (const std::invalid_argument &) {
    printTestResult("Constructor from array with null pointer throws exception",
                    true);
    ++testsPassed;
  } catch (...) {
    printTestResult(
        "Constructor from array with null pointer threw wrong exception",
        false);
    ++testsFailed;
  }
}

void testCopyConstructor() {
  int source[] = {10, 20, 30};
  DynamicArray<int> arr1(source, 3);
  DynamicArray<int> arr2(arr1);

  assertEqual(arr2.getSize(), 3, "Copy constructor: size");
  assertEqual(arr2.get(0), 10, "Copy constructor: first element");
  assertEqual(arr2.get(2), 30, "Copy constructor: last element");

  arr2.set(0, 999);
  assertEqual(arr1.get(0), 10,
              "Copy constructor: deep copy (original unchanged)");
}

void testAssignmentOperator() {
  int source1[] = {1, 2, 3};
  int source2[] = {10, 20, 30, 40};
  DynamicArray<int> arr1(source1, 3);
  DynamicArray<int> arr2(source2, 4);

  arr2 = arr1;

  assertEqual(arr2.getSize(), 3, "Assignment: size after assignment");
  assertEqual(arr2.get(0), 1, "Assignment: first element after assignment");
  assertEqual(arr2.get(2), 3, "Assignment: last element after assignment");

  arr2.set(0, 999);
  assertEqual(arr1.get(0), 1, "Assignment: deep copy (original unchanged)");
}

void testGetSet() {

  // Default:

  DynamicArray<int> arr(3);
  arr.set(0, 10);
  arr.set(1, 20);
  arr.set(2, 30);

  assertEqual(arr.get(0), 10, "get/set: first element");
  assertEqual(arr.get(1), 20, "get/set: second element");
  assertEqual(arr.get(2), 30, "get/set: third element");

  // Get out of range:

  try {
    arr.get(5);
    printTestResult("Get out of range did not throw exception", false);
    ++testsFailed;
  } catch (const std::out_of_range &) {
    printTestResult("Get out of range threw exception", true);
    ++testsPassed;
  }

  // Set out of range:

  try {
    arr.set(5, 555);
    printTestResult("Set out of range did not throw exception", false);
    ++testsFailed;
  } catch (const std::out_of_range &) {
    printTestResult("Set out of range threw exception", true);
    ++testsPassed;
  }
}

void testResize() {

  DynamicArray<int> arr(3);
  arr.set(0, 1);
  arr.set(1, 2);
  arr.set(2, 3);

  // Increase:

  arr.resize(5);
  assertEqual(arr.getSize(), 5, "Resize(increase): size");
  assertEqual(arr.get(0), 1, "Resize(increase): first element preserved");
  assertEqual(arr.get(2), 3, "Resize(increase): last old element preserved");

  // Decrease:

  arr.resize(2);
  assertEqual(arr.getSize(), 2, "Resize(decrease): size");
  assertEqual(arr.get(0), 1, "Resize(decrease): first element preserved");
  assertEqual(arr.get(1), 2, "Resize(decrease): last kept element preserved");

  // Negative:

  try {
    arr.resize(-5);
    printTestResult("Resize(negative size) did not throw exception", false);
    ++testsFailed;
  } catch (const std::invalid_argument &) {
    printTestResult("Resize negative size throws invalid_argument", true);
    ++testsPassed;
  } catch (...) {
    printTestResult("Resize(negative size) threw wrong exception", false);
    ++testsFailed;
  }

  // Removed elements:

  try {
    arr.get(2);
    printTestResult("Resize: no out-of-range for removed elements received",
                    false);
    ++testsFailed;
  } catch (const std::out_of_range &) {
    printTestResult("Resize: out-of-range for removed elements received", true);
    ++testsPassed;
  }

  // Zero:

  arr.resize(0);
  assertEqual(arr.getSize(), 0, "Resize(zero): size");
}
} // namespace DynamicArrayTests

namespace ArraySequenceTests {

void testMutableCreation() {
  MutableArraySequence<int> seq;
  assertEqual(seq.getLength(), 0, "MutableArraySequence default constructor");

  int arr[] = {1, 2, 3};
  MutableArraySequence<int> seq2(arr, 3);
  assertEqual(seq2.getLength(), 3, "MutableArraySequence from array");
  assertEqual(seq2.getFirst(), 1, "MutableArraySequence getFirst");
  assertEqual(seq2.getLast(), 3, "MutableArraySequence getLast");
}

void testImmutableCreation() {
  ImmutableArraySequence<int> seq;
  assertEqual(seq.getLength(), 0, "ImmutableArraySequence default constructor");

  int arr[] = {1, 2, 3};
  ImmutableArraySequence<int> seq2(arr, 3);
  assertEqual(seq2.getLength(), 3, "ImmutableArraySequence from array");
  assertEqual(seq2.getFirst(), 1, "ImmutableArraySequence getFirst");
  assertEqual(seq2.getLast(), 3, "ImmutableArraySequence getLast");
}

void testMutableAppendPrependInsertRemove() {
  MutableArraySequence<int> seq;
  seq.append(10);
  seq.append(20);
  seq.prepend(5);
  seq.insertAt(15, 2);
  seq.remove(1);

  assertEqual(seq.getLength(), 3, "MutableArraySequence after operations");
  assertEqual(seq.get(0), 5, "MutableArraySequence first element");
  assertEqual(seq.get(1), 15, "MutableArraySequence second element");
  assertEqual(seq.get(2), 20, "MutableArraySequence third element");
}

void testImmutableAppendPrependInsertRemove() {
  ImmutableArraySequence<int> seq;

  Sequence<int> *newSeq = seq.append(10);
  assertEqual(newSeq->getLength(), 1,
              "ImmutableArraySequence append result length");
  assertEqual(newSeq->get(0), 10, "ImmutableArraySequence append result value");
  delete newSeq;

  int arr[] = {1, 2, 3};
  ImmutableArraySequence<int> seq2(arr, 3);
  Sequence<int> *newSeq2 = seq2.prepend(0);
  assertEqual(newSeq2->getLength(), 4,
              "ImmutableArraySequence prepend result length");
  assertEqual(newSeq2->get(0), 0,
              "ImmutableArraySequence prepend result first");
  assertEqual(seq2.getLength(), 3,
              "ImmutableArraySequence original unchanged after prepend");
  delete newSeq2;

  Sequence<int> *newSeq3 = seq2.insertAt(99, 1);
  assertEqual(newSeq3->getLength(), 4,
              "ImmutableArraySequence insertAt result length");
  assertEqual(newSeq3->get(1), 99,
              "ImmutableArraySequence insertAt result value at index 1");
  delete newSeq3;

  Sequence<int> *newSeq4 = seq2.remove(1);
  assertEqual(newSeq4->getLength(), 2,
              "ImmutableArraySequence remove result length");
  assertEqual(newSeq4->get(0), 1, "ImmutableArraySequence remove result first");
  assertEqual(newSeq4->get(1), 3,
              "ImmutableArraySequence remove result second");
  assertEqual(seq2.getLength(), 3,
              "ImmutableArraySequence original unchanged after remove");
  delete newSeq4;
}

void testMutableGetSubsequence() {
  int arr[] = {1, 2, 3, 4, 5};
  MutableArraySequence<int> seq(arr, 5);
  Sequence<int> *sub = seq.getSubsequence(1, 3);
  assertEqual(sub->getLength(), 3,
              "MutableArraySequence getSubsequence length");
  assertEqual(sub->get(0), 2, "MutableArraySequence getSubsequence first");
  assertEqual(sub->get(1), 3, "MutableArraySequence getSubsequence second");
  assertEqual(sub->get(2), 4, "MutableArraySequence getSubsequence third");
  delete sub;
}

void testImmutableGetSubsequence() {
  int arr[] = {1, 2, 3, 4, 5};
  ImmutableArraySequence<int> seq(arr, 5);
  Sequence<int> *sub = seq.getSubsequence(1, 3);
  assertEqual(sub->getLength(), 3,
              "ImmutableArraySequence getSubsequence length");
  assertEqual(sub->get(0), 2, "ImmutableArraySequence getSubsequence first");
  assertEqual(seq.getLength(), 5,
              "ImmutableArraySequence original unchanged after getSubsequence");
  delete sub;
}

void testMutableConcat() {
  int arr1[] = {1, 2};
  int arr2[] = {3, 4};
  MutableArraySequence<int> seq1(arr1, 2);
  MutableArraySequence<int> seq2(arr2, 2);
  Sequence<int> *result = seq1.concat(seq2);
  assertEqual(result->getLength(), 4, "MutableArraySequence concat length");
  assertEqual(result->get(0), 1, "MutableArraySequence concat first");
  assertEqual(result->get(1), 2, "MutableArraySequence concat second");
  assertEqual(result->get(2), 3, "MutableArraySequence concat third");
  assertEqual(result->get(3), 4, "MutableArraySequence concat fourth");
  delete result;
}

void testImmutableConcat() {
  int arr1[] = {1, 2};
  int arr2[] = {3, 4};
  ImmutableArraySequence<int> seq1(arr1, 2);
  ImmutableArraySequence<int> seq2(arr2, 2);
  Sequence<int> *result = seq1.concat(seq2);
  assertEqual(result->getLength(), 4, "ImmutableArraySequence concat length");
  assertEqual(result->get(0), 1, "ImmutableArraySequence concat first");
  assertEqual(seq1.getLength(), 2,
              "ImmutableArraySequence original unchanged after concat");
  delete result;
}

void testExceptions() {
  MutableArraySequence<int> seq;
  try {
    seq.getFirst();
    printTestResult("getFirst on empty MutableArraySequence did not throw",
                    false);
    ++testsFailed;
  } catch (const std::out_of_range &) {
    printTestResult("getFirst on empty MutableArraySequence throws", true);
    ++testsPassed;
  }

  try {
    seq.getLast();
    printTestResult("getLast on empty MutableArraySequence did not throw",
                    false);
    ++testsFailed;
  } catch (const std::out_of_range &) {
    printTestResult("getLast on empty MutableArraySequence throws", true);
    ++testsPassed;
  }

  try {
    seq.get(0);
    printTestResult("get on empty MutableArraySequence did not throw", false);
    ++testsFailed;
  } catch (const std::out_of_range &) {
    printTestResult("get on empty MutableArraySequence throws", true);
    ++testsPassed;
  }

  try {
    seq.insertAt(5, -1);
    printTestResult("insertAt with negative index did not throw", false);
    ++testsFailed;
  } catch (const std::out_of_range &) {
    printTestResult("insertAt with negative index throws", true);
    ++testsPassed;
  }

  try {
    seq.insertAt(5, 10);
    printTestResult("insertAt with index > length did not throw", false);
    ++testsFailed;
  } catch (const std::out_of_range &) {
    printTestResult("insertAt with index > length throws", true);
    ++testsPassed;
  }

  try {
    seq.remove(0);
    printTestResult("remove on empty MutableArraySequence did not throw",
                    false);
    ++testsFailed;
  } catch (const std::out_of_range &) {
    printTestResult("remove on empty MutableArraySequence throws", true);
    ++testsPassed;
  }
}
} // namespace ArraySequenceTests

namespace LinkedListTests {

void testGetFirstLast() {
  int source[] = {10, 20, 30, 40};
  LinkedList<int> list(source, 4);

  assertEqual(list.getFirst(), 10, "Get first element");
  assertEqual(list.getLast(), 40, "Get last element");
  assertEqual(list.get(2), 30, "Get element with index 2");

  try {
    list.get(1000);
    printTestResult("Get out of range did not throw", false);
    ++testsFailed;
  } catch (const std::out_of_range &) {
    printTestResult("Get out of range throws exception", true);
    ++testsPassed;
  }

  LinkedList<int> listEmpty;

  try {
    listEmpty.getFirst();
    printTestResult("GetFirst on empty list did not throw", false);
    ++testsFailed;
  } catch (const std::out_of_range &) {
    printTestResult("GetFirst on empty list throws exception", true);
    ++testsPassed;
  }

  try {
    listEmpty.getLast();
    printTestResult("GetLast on empty list did not throw", false);
    ++testsFailed;
  } catch (const std::out_of_range &) {
    printTestResult("GetLast on empty list throws exception", true);
    ++testsPassed;
  }
}

void testDefaultConstructor() {
  LinkedList<int> list;
  assertEqual(list.getLength(), 0, "Default constructor creates empty list");
}

void testConstructorFromArray() {

  int source[] = {10, 20, 30, 40, 50};

  // Positive amount:

  LinkedList<int> listPos(source, 5);
  assertEqual(listPos.getLength(), 5,
              "Constructor from array with positive size");
  assertEqual(listPos.getFirst(), 10,
              "Constructor from array with positive size: first element");
  assertEqual(listPos.getLast(), 50,
              "Constructor from array with positive size: last element");

  // Negative amount:

  try {
    LinkedList<int> listNeg(source, -5);
    printTestResult("Constructor from array with negative size did not throw",
                    false);
    ++testsFailed;
  } catch (const std::invalid_argument &) {
    printTestResult(
        "Constructor from array with negative size throws exception", true);
    ++testsPassed;
  } catch (...) {
    printTestResult(
        "Constructor from array with negative size threw wrong exception",
        false);
    ++testsFailed;
  }

  // Zero amount:

  LinkedList<int> listZero(source, 0);
  assertEqual(listZero.getLength(), 0, "Constructor from array with zero size");

  // Null pointer:

  try {
    LinkedList<int> list(nullptr, 5);
    printTestResult("Constructor from array with null pointer did not throw",
                    false);
    ++testsFailed;
  } catch (const std::invalid_argument &) {
    printTestResult("Constructor from array with null pointer throws exception",
                    true);
    ++testsPassed;
  } catch (...) {
    printTestResult(
        "Constructor from array with null pointer threw wrong exception",
        false);
    ++testsFailed;
  }
}

void testCopyConstructor() {
  int source[] = {1, 2, 3};
  LinkedList<int> list1(source, 3);
  LinkedList<int> list2(list1);

  assertEqual(list2.getLength(), 3, "Copy constructor: size");
  assertEqual(list2.getFirst(), 1, "Copy constructor: first element");
  assertEqual(list2.getLast(), 3, "Copy constructor: last element");

  list2.prepend(999);
  assertEqual(list1.getFirst(), 1,
              "Copy constructor: deep copy (original unchanged)");
}

void testAssignmentOperator() {
  int source1[] = {1, 2, 3};
  int source2[] = {10, 20, 30, 40};
  LinkedList<int> list1(source1, 3);
  LinkedList<int> list2(source2, 4);

  list2 = list1;

  assertEqual(list2.getLength(), 3, "Assignment: size after assignment");
  assertEqual(list2.getFirst(), 1,
              "Assignment: first element after assignment");
  assertEqual(list2.getLast(), 3, "Assignment: last element after assignment");

  list2.prepend(999);
  assertEqual(list1.getFirst(), 1,
              "Assignment: deep copy (original unchanged)");
}

void testAppendPrepend() {
  LinkedList<int> list;

  list.append(10);
  list.append(20);
  list.prepend(5);
  list.prepend(1);

  assertEqual(list.getLength(), 4, "Append/prepend: size");
  assertEqual(list.getFirst(), 1,
              "Append/prepend: first element after prepend");
  assertEqual(list.getLast(), 20, "Append/prepend: last element after append");
  assertEqual(list.get(1), 5, "Append/prepend: element at index 1");
  assertEqual(list.get(2), 10, "Append/prepend: element at index 2");
}

void testInsertAt() {
  int source[] = {1, 2, 4, 5};
  LinkedList<int> list(source, 4);

  list.insertAt(3, 2);
  list.insertAt(0, 0);
  list.insertAt(6, list.getLength());

  assertEqual(list.getLength(), 7, "InsertAt: size");
  assertEqual(list.getFirst(), 0, "InsertAt: first element after insert at 0");
  assertEqual(list.get(2), 2, "InsertAt: element at index 2");
  assertEqual(list.get(3), 3, "InsertAt: element at index 3");
  assertEqual(list.getLast(), 6, "InsertAt: last element after insert at end");

  try {
    list.insertAt(99, 10);
    printTestResult("InsertAt out-of-range did not throw", false);
    ++testsFailed;
  } catch (const std::out_of_range &) {
    printTestResult("InsertAt out-of-range throws exception", true);
    ++testsPassed;
  } catch (...) {
    printTestResult("InsertAt out-of-range threw wrong exception", false);
    ++testsFailed;
  }
}

void testRemoveAt() {
  int source[] = {10, 20, 30, 40, 50};
  LinkedList<int> list(source, 5);

  list.removeAt(2);
  assertEqual(list.getLength(), 4, "RemoveAt: size after removing middle");
  assertEqual(list.get(1), 20, "RemoveAt: element at index 1 after removal");
  assertEqual(list.get(2), 40, "RemoveAt: element at index 2 after removal");

  list.removeAt(0);
  assertEqual(list.getLength(), 3, "RemoveAt: size after removing head");
  assertEqual(list.getFirst(), 20, "RemoveAt: new head after removing head");

  list.removeAt(list.getLength() - 1);
  assertEqual(list.getLength(), 2, "RemoveAt: size after removing tail");
  assertEqual(list.getLast(), 40, "RemoveAt: new tail after removing tail");

  LinkedList<int> single;
  single.append(100);
  single.removeAt(0);
  assertEqual(single.getLength(), 0,
              "RemoveAt: size after removing only element");

  try {
    list.removeAt(-5);
    printTestResult("RemoveAt negative index did not throw", false);
    ++testsFailed;
  } catch (const std::out_of_range &) {
    printTestResult("RemoveAt negative index throws exception", true);
    ++testsPassed;
  }

  try {
    list.removeAt(1000);
    printTestResult("RemoveAt index out of range did not throw", false);
    ++testsFailed;
  } catch (const std::out_of_range &) {
    printTestResult("RemoveAt index out of range throws exception", true);
    ++testsPassed;
  }
}

void testGetSublist() {

  int source[] = {1, 2, 3, 4, 5, 6};
  LinkedList<int> list(source, 6);
  LinkedList<int> sub = list.getSublist(1, 3);

  assertEqual(sub.getLength(), 3, "GetSublist: size");
  assertEqual(sub.getFirst(), 2, "GetSublist: first element");
  assertEqual(sub.getLast(), 4, "GetSublist: last element");
  assertEqual(sub.get(1), 3, "GetSublist: element at index 1");

  try {
    list.getSublist(-1, 2);
    printTestResult("GetSublist negative start did not throw exception", false);
    ++testsFailed;
  } catch (const std::out_of_range &) {
    printTestResult("GetSublist negative start throws exception", true);
    ++testsPassed;
  } catch (...) {
    printTestResult("GetSublist negative start threw wrong exception", false);
    ++testsFailed;
  }

  try {
    list.getSublist(2, 10);
    printTestResult("GetSublist end out of range did not throw exception",
                    false);
    ++testsFailed;
  } catch (const std::out_of_range &) {
    printTestResult("GetSublist end out of range throws exception", true);
    ++testsPassed;
  } catch (...) {
    printTestResult("GetSublist end out of range threw wrong exception", false);
    ++testsFailed;
  }

  try {
    list.getSublist(4, 2);
    printTestResult("GetSublist start > end did not throw", false);
    ++testsFailed;
  } catch (const std::out_of_range &) {
    printTestResult("GetSublist start > end throws", true);
    ++testsPassed;
  } catch (...) {
    printTestResult("GetSublist start > end threw wrong exception", false);
    ++testsFailed;
  }
}

void testLinkedListConcat() {
  int source1[] = {1, 2, 3};
  int source2[] = {4, 5, 6};
  LinkedList<int> list1(source1, 3);
  LinkedList<int> list2(source2, 3);

  LinkedList<int> result = list1.concat(list2);
  assertEqual(result.getLength(), 6, "LinkedList concat: size");
  assertEqual(result.getFirst(), 1, "LinkedList concat: first element");
  assertEqual(result.getLast(), 6, "LinkedList concat: last element");
  assertEqual(result.get(2), 3, "LinkedList concat: element at index 2");
  assertEqual(result.get(3), 4, "LinkedList concat: element at index 3");
}
} // namespace LinkedListTests

namespace ListSequenceTests {

void testMutableCreation() {
  MutableListSequence<int> seq;
  assertEqual(seq.getLength(), 0, "MutableListSequence default constructor");

  int arr[] = {1, 2, 3};
  MutableListSequence<int> seq2(arr, 3);
  assertEqual(seq2.getLength(), 3, "MutableListSequence from array");
  assertEqual(seq2.getFirst(), 1, "MutableListSequence getFirst");
  assertEqual(seq2.getLast(), 3, "MutableListSequence getLast");
}

void testImmutableCreation() {
  ImmutableListSequence<int> seq;
  assertEqual(seq.getLength(), 0, "ImmutableListSequence default constructor");

  int arr[] = {1, 2, 3};
  ImmutableListSequence<int> seq2(arr, 3);
  assertEqual(seq2.getLength(), 3, "ImmutableListSequence from array");
  assertEqual(seq2.getFirst(), 1, "ImmutableListSequence getFirst");
  assertEqual(seq2.getLast(), 3, "ImmutableListSequence getLast");
}

void testMutableAppendPrependInsertRemove() {
  MutableListSequence<int> seq;
  seq.append(10);
  seq.append(20);
  seq.prepend(5);
  seq.insertAt(15, 2);
  seq.remove(1);

  assertEqual(seq.getLength(), 3, "MutableListSequence after operations");
  assertEqual(seq.get(0), 5, "MutableListSequence first element");
  assertEqual(seq.get(1), 15, "MutableListSequence second element");
  assertEqual(seq.get(2), 20, "MutableListSequence third element");
}

void testImmutableAppendPrependInsertRemove() {
  ImmutableListSequence<int> seq;
  assertEqual(seq.getLength(), 0,
              "ImmutableListSequence original unchanged after append");

  Sequence<int> *newSeq = seq.append(10);
  assertEqual(newSeq->getLength(), 1,
              "ImmutableListSequence append result length");
  assertEqual(newSeq->get(0), 10, "ImmutableListSequence append result value");
  delete newSeq;

  int arr[] = {1, 2, 3};
  ImmutableListSequence<int> seq2(arr, 3);

  Sequence<int> *newSeq2 = seq2.prepend(0);
  assertEqual(newSeq2->getLength(), 4,
              "ImmutableListSequence prepend result length");
  assertEqual(newSeq2->get(0), 0, "ImmutableListSequence prepend result first");
  assertEqual(seq2.getLength(), 3,
              "ImmutableListSequence original unchanged after prepend");
  delete newSeq2;

  Sequence<int> *newSeq3 = seq2.insertAt(99, 1);
  assertEqual(newSeq3->getLength(), 4,
              "ImmutableListSequence insertAt result length");
  assertEqual(newSeq3->get(1), 99,
              "ImmutableListSequence insertAt result value");
  delete newSeq3;

  Sequence<int> *newSeq4 = seq2.remove(1);
  assertEqual(newSeq4->getLength(), 2,
              "ImmutableListSequence remove result length");
  assertEqual(newSeq4->get(0), 1, "ImmutableListSequence remove result first");
  assertEqual(newSeq4->get(1), 3, "ImmutableListSequence remove result second");
  assertEqual(seq2.getLength(), 3,
              "ImmutableListSequence original unchanged after remove");
  delete newSeq4;
}

void testMutableGetSubsequence() {
  int arr[] = {1, 2, 3, 4, 5};
  MutableListSequence<int> seq(arr, 5);
  Sequence<int> *sub = seq.getSubsequence(1, 3);
  assertEqual(sub->getLength(), 3, "MutableListSequence getSubsequence length");
  assertEqual(sub->get(0), 2, "MutableListSequence getSubsequence first");
  assertEqual(sub->get(1), 3, "MutableListSequence getSubsequence second");
  assertEqual(sub->get(2), 4, "MutableListSequence getSubsequence third");
  delete sub;
}

void testImmutableGetSubsequence() {
  int arr[] = {1, 2, 3, 4, 5};
  ImmutableListSequence<int> seq(arr, 5);
  Sequence<int> *sub = seq.getSubsequence(1, 3);
  assertEqual(sub->getLength(), 3,
              "ImmutableListSequence getSubsequence length");
  assertEqual(sub->get(0), 2, "ImmutableListSequence getSubsequence first");
  assertEqual(seq.getLength(), 5,
              "ImmutableListSequence original unchanged after getSubsequence");
  delete sub;
}

void testMutableConcat() {
  int arr1[] = {1, 2};
  int arr2[] = {3, 4};
  MutableListSequence<int> seq1(arr1, 2);
  MutableListSequence<int> seq2(arr2, 2);
  Sequence<int> *result = seq1.concat(seq2);
  assertEqual(result->getLength(), 4, "MutableListSequence concat length");
  assertEqual(result->get(0), 1, "MutableListSequence concat first");
  assertEqual(result->get(1), 2, "MutableListSequence concat second");
  assertEqual(result->get(2), 3, "MutableListSequence concat third");
  assertEqual(result->get(3), 4, "MutableListSequence concat fourth");
  delete result;
}

void testImmutableConcat() {
  int arr1[] = {1, 2};
  int arr2[] = {3, 4};
  ImmutableListSequence<int> seq1(arr1, 2);
  ImmutableListSequence<int> seq2(arr2, 2);
  Sequence<int> *result = seq1.concat(seq2);
  assertEqual(result->getLength(), 4, "ImmutableListSequence concat length");
  assertEqual(result->get(0), 1, "ImmutableListSequence concat first");
  assertEqual(seq1.getLength(), 2,
              "ImmutableListSequence original unchanged after concat");
  delete result;
}

void testExceptions() {
  MutableListSequence<int> seq;
  try {
    seq.getFirst();
    printTestResult("getFirst on empty MutableListSequence did not throw",
                    false);
    ++testsFailed;
  } catch (const std::out_of_range &) {
    printTestResult("getFirst on empty MutableListSequence throws", true);
    ++testsPassed;
  }

  try {
    seq.getLast();
    printTestResult("getLast on empty MutableListSequence did not throw",
                    false);
    ++testsFailed;
  } catch (const std::out_of_range &) {
    printTestResult("getLast on empty MutableListSequence throws", true);
    ++testsPassed;
  }

  try {
    seq.get(0);
    printTestResult("get on empty MutableListSequence did not throw", false);
    ++testsFailed;
  } catch (const std::out_of_range &) {
    printTestResult("get on empty MutableListSequence throws", true);
    ++testsPassed;
  }

  try {
    seq.insertAt(5, -1);
    printTestResult("insertAt negative index did not throw", false);
    ++testsFailed;
  } catch (const std::out_of_range &) {
    printTestResult("insertAt negative index throws", true);
    ++testsPassed;
  }

  try {
    seq.insertAt(5, 10);
    printTestResult("insertAt index > length did not throw", false);
    ++testsFailed;
  } catch (const std::out_of_range &) {
    printTestResult("insertAt index > length throws", true);
    ++testsPassed;
  }

  try {
    seq.remove(0);
    printTestResult("remove on empty MutableListSequence did not throw", false);
    ++testsFailed;
  } catch (const std::out_of_range &) {
    printTestResult("remove on empty MutableListSequence throws", true);
    ++testsPassed;
  }
}
} // namespace ListSequenceTests

namespace VectorTests {

void testConstructors() {
  // int
  Vector<int> i1;
  assertEqual(i1.getSize(), 0, "int: default constructor");

  Vector<int> i2(3);
  assertEqual(i2.getSize(), 3, "int: size constructor");

  int iarr[] = {1, 2, 3};
  Vector<int> i3(iarr, 3);
  assertEqual(i3.getSize(), 3, "int: from array size");
  assertEqual(i3.get(0), 1, "int: from array first");
  assertEqual(i3.get(2), 3, "int: from array last");

  MutableArraySequence<int> iseq(iarr, 3);
  Vector<int> i4(iseq);
  assertEqual(i4.getSize(), 3, "int: from Sequence size");
  assertEqual(i4.get(0), 1, "int: from Sequence first");

  // double
  Vector<double> d1;
  assertEqual(d1.getSize(), 0, "double: default constructor");

  Vector<double> d2(3);
  assertEqual(d2.getSize(), 3, "double: size constructor");

  double darr[] = {1.1, 2.2, 3.3};
  Vector<double> d3(darr, 3);
  assertEqual(d3.getSize(), 3, "double: from array size");
  assertEqual(d3.get(0), 1.1, "double: from array first");
  assertEqual(d3.get(2), 3.3, "double: from array last");

  MutableArraySequence<double> dseq(darr, 3);
  Vector<double> d4(dseq);
  assertEqual(d4.getSize(), 3, "double: from Sequence size");
  assertEqual(d4.get(0), 1.1, "double: from Sequence first");

  // complex
  Vector<std::complex<double>> c1;
  assertEqual(c1.getSize(), 0, "complex: default constructor");

  Vector<std::complex<double>> c2(3);
  assertEqual(c2.getSize(), 3, "complex: size constructor");

  std::complex<double> carr[] = {{1, 2}, {3, 4}, {5, 6}};
  Vector<std::complex<double>> c3(carr, 3);
  assertEqual(c3.getSize(), 3, "complex: from array size");
  assertEqual(c3.get(0), std::complex<double>(1, 2),
              "complex: from array first");
  assertEqual(c3.get(2), std::complex<double>(5, 6),
              "complex: from array last");

  MutableArraySequence<std::complex<double>> cseq(carr, 3);
  Vector<std::complex<double>> c4(cseq);
  assertEqual(c4.getSize(), 3, "complex: from Sequence size");
  assertEqual(c4.get(0), std::complex<double>(1, 2),
              "complex: from Sequence first");
}

void testGetSet() {
  // int
  Vector<int> i(3);
  i.set(10, 0);
  i.set(20, 1);
  i.set(30, 2);
  assertEqual(i.get(0), 10, "int: get/set first");
  assertEqual(i.get(1), 20, "int: get/set second");
  assertEqual(i.get(2), 30, "int: get/set third");

  // double
  Vector<double> d(3);
  d.set(1.5, 0);
  d.set(2.5, 1);
  d.set(3.5, 2);
  assertEqual(d.get(0), 1.5, "double: get/set first");
  assertEqual(d.get(1), 2.5, "double: get/set second");
  assertEqual(d.get(2), 3.5, "double: get/set third");

  // complex
  Vector<std::complex<double>> c(3);
  c.set({1, 2}, 0);
  c.set({3, 4}, 1);
  c.set({5, 6}, 2);
  assertEqual(c.get(0), std::complex<double>(1, 2), "complex: get/set first");
  assertEqual(c.get(1), std::complex<double>(3, 4), "complex: get/set second");
  assertEqual(c.get(2), std::complex<double>(5, 6), "complex: get/set third");
}

void testAdd() {
  // int
  int i1[] = {1, 2, 3};
  int i2[] = {4, 5, 6};
  Vector<int> ia(i1, 3);
  Vector<int> ib(i2, 3);
  Vector<int> isum = ia.add(ib);
  assertEqual(isum.getSize(), 3, "int: add size");
  assertEqual(isum.get(0), 5, "int: add first");
  assertEqual(isum.get(1), 7, "int: add second");
  assertEqual(isum.get(2), 9, "int: add third");

  // double
  double d1[] = {1.0, 2.0, 3.0};
  double d2[] = {0.5, 1.5, 2.5};
  Vector<double> da(d1, 3);
  Vector<double> db(d2, 3);
  Vector<double> dsum = da.add(db);
  assertEqual(dsum.getSize(), 3, "double: add size");
  assertEqual(dsum.get(0), 1.5, "double: add first");
  assertEqual(dsum.get(1), 3.5, "double: add second");
  assertEqual(dsum.get(2), 5.5, "double: add third");

  // complex
  std::complex<double> c1[] = {{1, 1}, {2, 2}};
  std::complex<double> c2[] = {{0.5, 0.5}, {1.5, 1.5}};
  Vector<std::complex<double>> ca(c1, 2);
  Vector<std::complex<double>> cb(c2, 2);
  Vector<std::complex<double>> csum = ca.add(cb);
  assertEqual(csum.getSize(), 2, "complex: add size");
  assertEqual(csum.get(0), std::complex<double>(1.5, 1.5),
              "complex: add first");
  assertEqual(csum.get(1), std::complex<double>(3.5, 3.5),
              "complex: add second");
}

void testScalarMul() {
  // int
  int iarr[] = {1, 2, 3};
  Vector<int> i(iarr, 3);
  Vector<int> ires = i.scalarMul(2);
  assertEqual(ires.getSize(), 3, "int: scalarMul size");
  assertEqual(ires.get(0), 2, "int: scalarMul first");
  assertEqual(ires.get(1), 4, "int: scalarMul second");
  assertEqual(ires.get(2), 6, "int: scalarMul third");

  // double
  double darr[] = {1.0, 2.0, 3.0};
  Vector<double> d(darr, 3);
  Vector<double> dres = d.scalarMul(2.0);
  assertEqual(dres.getSize(), 3, "double: scalarMul size");
  assertEqual(dres.get(0), 2.0, "double: scalarMul first");
  assertEqual(dres.get(1), 4.0, "double: scalarMul second");
  assertEqual(dres.get(2), 6.0, "double: scalarMul third");

  // complex
  std::complex<double> carr[] = {{1, 1}, {2, 2}};
  Vector<std::complex<double>> c(carr, 2);
  Vector<std::complex<double>> cres = c.scalarMul(2.0);
  assertEqual(cres.getSize(), 2, "complex: scalarMul size");
  assertEqual(cres.get(0), std::complex<double>(2.0, 2.0),
              "complex: scalarMul first");
  assertEqual(cres.get(1), std::complex<double>(4.0, 4.0),
              "complex: scalarMul second");
}

void testDotProduct() {
  // int
  int i1[] = {1, 2, 3};
  int i2[] = {4, 5, 6};
  Vector<int> ia(i1, 3);
  Vector<int> ib(i2, 3);
  int idot = ia.dotProduct(ib);
  assertEqual(idot, 32, "int: dotProduct");

  // double
  double d1[] = {1.0, 2.0, 3.0};
  double d2[] = {0.5, 1.5, 2.5};
  Vector<double> da(d1, 3);
  Vector<double> db(d2, 3);
  double ddot = da.dotProduct(db);
  assertEqual(ddot, 11.0, "double: dotProduct");

  // complex
  std::complex<double> c1[] = {{1, 1}, {2, 2}};
  std::complex<double> c2[] = {{0.5, 0.5}, {1.5, 1.5}};
  Vector<std::complex<double>> ca(c1, 2);
  Vector<std::complex<double>> cb(c2, 2);
  std::complex<double> cdot = ca.dotProduct(cb);
  assertEqual(cdot, std::complex<double>(0.0, 7.0), "complex: dotProduct");
}

void testNorm() {
  // int
  int iarr[] = {3, 4};
  Vector<int> i(iarr, 2);
  double in = i.norm();
  assertEqual(in, 5.0, "int: norm (3,4)");

  // double
  double darr[] = {3.0, 4.0};
  Vector<double> d(darr, 2);
  double dn = d.norm();
  assertEqual(dn, 5.0, "double: norm (3,4)");

  // complex
  std::complex<double> carr[] = {{3, 4}, {0, 0}};
  Vector<std::complex<double>> c(carr, 2);
  double cn = c.norm();
  assertEqual(cn, 5.0, "complex: norm (3+4i,0)");
}

void testExceptions() {
  // int
  Vector<int> i1(2);
  Vector<int> i2(3);
  try {
    i1.add(i2);
    printTestResult("int: add diff sizes did not throw", false);
    ++testsFailed;
  } catch (const std::invalid_argument &) {
    printTestResult("int: add diff sizes throws", true);
    ++testsPassed;
  }
  try {
    i1.dotProduct(i2);
    printTestResult("int: dot diff sizes did not throw", false);
    ++testsFailed;
  } catch (const std::invalid_argument &) {
    printTestResult("int: dot diff sizes throws", true);
    ++testsPassed;
  }

  // double
  Vector<double> d1(2);
  Vector<double> d2(3);
  try {
    d1.add(d2);
    printTestResult("double: add diff sizes did not throw", false);
    ++testsFailed;
  } catch (const std::invalid_argument &) {
    printTestResult("double: add diff sizes throws", true);
    ++testsPassed;
  }
  try {
    d1.dotProduct(d2);
    printTestResult("double: dot diff sizes did not throw", false);
    ++testsFailed;
  } catch (const std::invalid_argument &) {
    printTestResult("double: dot diff sizes throws", true);
    ++testsPassed;
  }

  // complex
  Vector<std::complex<double>> c1(2);
  Vector<std::complex<double>> c2(3);
  try {
    c1.add(c2);
    printTestResult("complex: add diff sizes did not throw", false);
    ++testsFailed;
  } catch (const std::invalid_argument &) {
    printTestResult("complex: add diff sizes throws", true);
    ++testsPassed;
  }
  try {
    c1.dotProduct(c2);
    printTestResult("complex: dot diff sizes did not throw", false);
    ++testsFailed;
  } catch (const std::invalid_argument &) {
    printTestResult("complex: dot diff sizes throws", true);
    ++testsPassed;
  }
}

} // namespace VectorTests

namespace ExtraTasksTests {

void testCountInversions() {
  // int
  int arr1[] = {3, 1, 2};
  MutableArraySequence<int> seq1(arr1, 3);
  assertEqual(countInversions(seq1), 2, "countInversions: [3,1,2]");

  int arr2[] = {1, 2, 3};
  MutableArraySequence<int> seq2(arr2, 3);
  assertEqual(countInversions(seq2), 0, "countInversions: [1,2,3]");

  int arr3[] = {3, 2, 1};
  MutableArraySequence<int> seq3(arr3, 3);
  assertEqual(countInversions(seq3), 3, "countInversions: [3,2,1]");

  // string
  std::string sarr[] = {"b", "a", "c"};
  MutableArraySequence<std::string> sseq(sarr, 3);
  assertEqual(countInversions(sseq), 1, "countInversions: [b,a,c]");
}

void testAllCombinationsEmpty() {
  MutableArraySequence<int> seq;
  auto result = allCombinations(seq);
  assertEqual(result->getLength(), 1, "allCombinations: empty - total");
  assertEqual(result->get(0).getLength(), 0,
              "allCombinations: empty - only empty");
  delete result;
}

void testAllCombinationsOne() {
  int arr[] = {5};
  MutableArraySequence<int> seq(arr, 1);
  auto result = allCombinations(seq);
  assertEqual(result->getLength(), 2, "allCombinations: [5] total");
  int len0 = 0, len1 = 0;
  for (int i = 0; i < result->getLength(); ++i) {
    int len = result->get(i).getLength();
    if (len == 0)
      len0++;
    else if (len == 1)
      len1++;
  }
  assertEqual(len0, 1, "allCombinations: [5] len0 count");
  assertEqual(len1, 1, "allCombinations: [5] len1 count");
  delete result;
}

void testAllCombinationsUnique() {
  int arr[] = {1, 2, 3};
  MutableArraySequence<int> seq(arr, 3);
  auto result = allCombinations(seq);
  assertEqual(result->getLength(), 8, "allCombinations: [1,2,3] total");
  int len0 = 0, len1 = 0, len2 = 0, len3 = 0;
  for (int i = 0; i < result->getLength(); ++i) {
    int len = result->get(i).getLength();
    if (len == 0)
      len0++;
    else if (len == 1)
      len1++;
    else if (len == 2)
      len2++;
    else if (len == 3)
      len3++;
  }
  assertEqual(len0, 1, "allCombinations: [1,2,3] len0");
  assertEqual(len1, 3, "allCombinations: [1,2,3] len1");
  assertEqual(len2, 3, "allCombinations: [1,2,3] len2");
  assertEqual(len3, 1, "allCombinations: [1,2,3] len3");
  delete result;
}

void testAllCombinationsWithDups() {
  int arr[] = {1, 2, 2};
  MutableArraySequence<int> seq(arr, 3);
  auto result = allCombinations(seq);
  // Уникальные по значению: [], [1], [2], [1,2], [2,2], [1,2,2] → всего 6
  assertEqual(result->getLength(), 6, "allCombinations: [1,2,2] total");
  int len0 = 0, len1 = 0, len2 = 0, len3 = 0;
  for (int i = 0; i < result->getLength(); ++i) {
    int len = result->get(i).getLength();
    if (len == 0)
      len0++;
    else if (len == 1)
      len1++;
    else if (len == 2)
      len2++;
    else if (len == 3)
      len3++;
  }
  assertEqual(len0, 1, "allCombinations: [1,2,2] len0");
  assertEqual(len1, 2, "allCombinations: [1,2,2] len1");
  assertEqual(len2, 2, "allCombinations: [1,2,2] len2");
  assertEqual(len3, 1, "allCombinations: [1,2,2] len3");
  delete result;
}

void testAllCombinationsAllSame() {
  int arr[] = {2, 2, 2};
  MutableArraySequence<int> seq(arr, 3);
  auto result = allCombinations(seq);
  // Уникальные: [], [2], [2,2], [2,2,2] → всего 4
  assertEqual(result->getLength(), 4, "allCombinations: [2,2,2] total");
  int len0 = 0, len1 = 0, len2 = 0, len3 = 0;
  for (int i = 0; i < result->getLength(); ++i) {
    int len = result->get(i).getLength();
    if (len == 0)
      len0++;
    else if (len == 1)
      len1++;
    else if (len == 2)
      len2++;
    else if (len == 3)
      len3++;
  }
  assertEqual(len0, 1, "allCombinations: [2,2,2] len0");
  assertEqual(len1, 1, "allCombinations: [2,2,2] len1");
  assertEqual(len2, 1, "allCombinations: [2,2,2] len2");
  assertEqual(len3, 1, "allCombinations: [2,2,2] len3");
  delete result;
}

void testAllSubsequences() {
  int arr[] = {1, 2, 3};
  MutableArraySequence<int> seq(arr, 3);
  auto result = allSubsequences(seq);
  assertEqual(result->getLength(), 7, "allSubsequences: length");

  int len = 0, countLen0 = 0, countLen1 = 0, countLen2 = 0, countLen3 = 0;
  for (int i = 0; i < result->getLength(); ++i) {
    len = result->get(i).getLength();
    if (len == 0)
      ++countLen0;
    else if (len == 1)
      ++countLen1;
    else if (len == 2)
      ++countLen2;
    else if (len == 3)
      ++countLen3;
  }
  assertEqual(countLen0, 1, "allSubsequences: empty subset");
  assertEqual(countLen1, 3, "allSubsequences: subset of length 1");
  assertEqual(countLen2, 2, "allSubsequences: subset of length 2");
  assertEqual(countLen3, 1, "allSubsequences: subset of length 3");
  delete result;
}

} // namespace ExtraTasksTests

void runAllTests() {
  std::cout << "====== TESTS ======" << std::endl;

  std::cout << "\nDynamicArray:\n" << std::endl;
  {
    using namespace DynamicArrayTests;
    testDefaultConstructor();
    testSizeConstructor();
    testConstructorFromArray();
    testCopyConstructor();
    testAssignmentOperator();
    testGetSet();
    testResize();
  }

  std::cout << "\nArraySequence:\n" << std::endl;
  {
    using namespace ArraySequenceTests;
    testMutableCreation();
    testImmutableCreation();
    testMutableAppendPrependInsertRemove();
    testImmutableAppendPrependInsertRemove();
    testMutableGetSubsequence();
    testImmutableGetSubsequence();
    testMutableConcat();
    testImmutableConcat();
    testExceptions();
  }

  std::cout << "\nLinkedList:\n" << std::endl;
  {
    using namespace LinkedListTests;
    testGetFirstLast();
    testDefaultConstructor();
    testConstructorFromArray();
    testCopyConstructor();
    testAssignmentOperator();
    testAppendPrepend();
    testInsertAt();
    testRemoveAt();
    testGetSublist();
    testLinkedListConcat();
  }

  std::cout << "\nListSequence:\n" << std::endl;
  {
    using namespace ListSequenceTests;
    testMutableCreation();
    testImmutableCreation();
    testMutableAppendPrependInsertRemove();
    testImmutableAppendPrependInsertRemove();
    testMutableGetSubsequence();
    testImmutableGetSubsequence();
    testMutableConcat();
    testImmutableConcat();
    testExceptions();
  }

  std::cout << "\nVector:\n" << std::endl;
  {
    using namespace VectorTests;
    testConstructors();
    testGetSet();
    testAdd();
    testScalarMul();
    testDotProduct();
    testNorm();
    testExceptions();
  }

  std::cout << "\nExtra tasks:\n" << std::endl;
  {
    using namespace ExtraTasksTests;
    testCountInversions();
    testAllCombinationsEmpty();
    testAllCombinationsOne();
    testAllCombinationsUnique();
    testAllCombinationsWithDups();
    testAllCombinationsAllSame();
    testAllSubsequences();
  }

  std::cout << "\n=== Results ===" << std::endl;
  std::cout << testsPassed << "/" << testsPassed + testsFailed
            << " tests passed." << std::endl;
}