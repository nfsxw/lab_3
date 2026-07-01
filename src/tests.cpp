#include <complex>
#include <iomanip>
#include <iostream>
#include <string>

#include "ArraySequence.hpp"
#include "DynamicArray.hpp"
#include "LinkedList.hpp"
#include "ListSequence.hpp"
#include "Vector.hpp"
#include "Permutations.hpp"

int testsPassed = 0;
int testsFailed = 0;

namespace std {
string to_string(const complex<double> &c) {
  return "(" + to_string(c.real()) + "," + to_string(c.imag()) + ")";
}
} // namespace std

void printTestResult(const std::string &testName, bool passed) {
  const int width = 80;
  std::cout << std::left << " - " << std::setw(width) << testName;
  if (passed) {
    std::cout << "\033[32m [PASSED] \033[0m" << std::endl;
  } else {
    std::cout << "\033[31m [FAILED] \033[0m" << std::endl;
  }
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
    printTestResult("Size constructor negative size throws exception", true);
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

  arr2.set(999, 0);
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

  arr2.set(67, 0);
  assertEqual(arr1.get(0), 1, "Assignment: deep copy (original unchanged)");
}

void testGetSet() {

  // Default:

  DynamicArray<int> arr(3);
  arr.set(10, 0);
  arr.set(20, 1);
  arr.set(30, 2);

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
    arr.set(555, 5);
    printTestResult("Set out of range did not throw exception", false);
    ++testsFailed;
  } catch (const std::out_of_range &) {
    printTestResult("Set out of range threw exception", true);
    ++testsPassed;
  }
}

void testResize() {

  DynamicArray<int> arr(3);
  arr.set(1, 0);
  arr.set(2, 1);
  arr.set(3, 2);

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

namespace LinkedListTests {

void testDefaultConstructor() {
  LinkedList<int> list;
  assertEqual(list.getSize(), 0, "Default constructor creates empty list");
}

void testConstructorFromArray() {

  int source[] = {10, 20, 30, 40, 50};

  // Positive amount:

  LinkedList<int> listPos(source, 5);
  assertEqual(listPos.getSize(), 5,
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
  assertEqual(listZero.getSize(), 0, "Constructor from array with zero size");

  // Null pointer:

  try {
    LinkedList<int> listNullPtr(nullptr, 5);
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

  assertEqual(list2.getSize(), 3, "Copy constructor: size");
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

  assertEqual(list2.getSize(), 3, "Assignment: size after assignment");
  assertEqual(list2.getFirst(), 1,
              "Assignment: first element after assignment");
  assertEqual(list2.getLast(), 3, "Assignment: last element after assignment");

  list2.prepend(999);
  assertEqual(list1.getFirst(), 1,
              "Assignment: deep copy (original unchanged)");
}

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

void testSet() {
  int source[] = {1, 2, 3};
  LinkedList<int> list(source, 3);

  list.set(67, 0);
  assertEqual(list.getFirst(), 67, "LinkedList set: first element");

  list.set(42, 1);
  assertEqual(list.get(1), 42, "LinkedList set: middle element");

  list.set(52, 2);
  assertEqual(list.getLast(), 52, "LinkedList set: last element");

  // Out of range
  try {
    list.set(555, 10);
    printTestResult("LinkedList set out of range did not throw", false);
    ++testsFailed;
  } catch (const std::out_of_range &) {
    printTestResult("LinkedList set out of range throws exception", true);
    ++testsPassed;
  } catch (...) {
    printTestResult("LinkedList set out of range threw wrong exception", false);
    ++testsFailed;
  }

  // Negative index
  try {
    list.set(555, -5);
    printTestResult("LinkedList set negative index did not throw", false);
    ++testsFailed;
  } catch (const std::out_of_range &) {
    printTestResult("LinkedList set negative index throws exception", true);
    ++testsPassed;
  } catch (...) {
    printTestResult("LinkedList set negative index threw wrong exception",
                    false);
    ++testsFailed;
  }
}

void testAppendPrepend() {
  LinkedList<int> list;

  list.append(10);
  list.append(20);
  list.prepend(5);
  list.prepend(1);

  assertEqual(list.getSize(), 4, "Append/prepend: size");
  assertEqual(list.getFirst(), 1, "Append/prepend: first element");
  assertEqual(list.get(1), 5, "Append/prepend: element at index 1");
  assertEqual(list.get(2), 10, "Append/prepend: element at index 2");
  assertEqual(list.getLast(), 20, "Append/prepend: last element");
}

void testInsertAt() {
  int source[] = {1, 2, 4, 5};
  LinkedList<int> list(source, 4);

  list.insertAt(3, 2);
  list.insertAt(0, 0);
  list.insertAt(6, list.getSize());

  assertEqual(list.getSize(), 7, "InsertAt: size");
  assertEqual(list.getFirst(), 0, "InsertAt: first element");
  assertEqual(list.get(2), 2, "InsertAt: element at index 2");
  assertEqual(list.get(3), 3, "InsertAt: element at index 3");
  assertEqual(list.getLast(), 6, "InsertAt: last element");

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
  assertEqual(list.getSize(), 4, "RemoveAt: size after removing middle");
  assertEqual(list.get(1), 20, "RemoveAt: element at index 1 after removal");
  assertEqual(list.get(2), 40, "RemoveAt: element at index 2 after removal");

  list.removeAt(0);
  assertEqual(list.getSize(), 3, "RemoveAt: size after removing head");
  assertEqual(list.getFirst(), 20, "RemoveAt: new head after removing head");

  list.removeAt(list.getSize() - 1);
  assertEqual(list.getSize(), 2, "RemoveAt: size after removing tail");
  assertEqual(list.getLast(), 40, "RemoveAt: new tail after removing tail");

  LinkedList<int> single;
  single.append(100);
  single.removeAt(0);
  assertEqual(single.getSize(), 0,
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

  assertEqual(sub.getSize(), 3, "GetSublist: size");
  assertEqual(sub.getFirst(), 2, "GetSublist: first element");
  assertEqual(sub.get(1), 3, "GetSublist: element at index 1");
  assertEqual(sub.getLast(), 4, "GetSublist: last element");

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
  assertEqual(result.getSize(), 6, "LinkedList concat: size");
  assertEqual(result.getFirst(), 1, "LinkedList concat: first element");
  assertEqual(result.get(2), 3, "LinkedList concat: element at index 2");
  assertEqual(result.getLast(), 6, "LinkedList concat: last element");
}
} // namespace LinkedListTests

namespace ArraySequenceTests {

void testDefaultConstructor() {
  ArraySequence<int> seq;
  assertEqual(seq.getSize(), 0, "ArraySequence default constructor");
}

void testSizeConstructor() {
  ArraySequence<int> seq(3);
  assertEqual(seq.getSize(), 3, "ArraySequence size constructor");
}

void testConstructorFromArray() {
  int arr[] = {1, 2, 3};
  ArraySequence<int> seq(arr, 3);
  assertEqual(seq.getSize(), 3, "ArraySequence constructor from array: size");
  assertEqual(seq.getFirst(), 1,
              "ArraySequence constructor from array: getFirst");
  assertEqual(seq.getLast(), 3,
              "ArraySequence constructor from array: getLast");
}

void testCopyConstructor() {
  int arr1[] = {1, 2, 3};
  ArraySequence<int> seq1(arr1, 3);
  ArraySequence<int> seq2(seq1);
  assertEqual(seq2.getSize(), seq1.getSize(),
              "ArraySequence copy constructor: size");
  assertEqual(seq2.getFirst(), seq1.getFirst(),
              "ArraySequence copy constructor: first");
  assertEqual(seq2.getLast(), seq1.getLast(),
              "ArraySequence copy constructor: last");

  seq2.prepend(67);
  assertEqual(
      seq1.getFirst(), 1,
      "ArraySequence copy constructor: deep copy (original not changed)");
}

void testAssignmentOperator() {
  int arr1[] = {1, 2, 3};
  int arr2[] = {4, 5, 6, 7};
  ArraySequence<int> seq1(arr1, 3);
  ArraySequence<int> seq2(arr2, 4);
  seq2 = seq1;

  assertEqual(seq2.getSize(), seq1.getSize(),
              "ArraySequence AssignmentOperator: size");
  assertEqual(seq2.getFirst(), seq1.getFirst(),
              "ArraySequence AssignmentOperator: first");
  assertEqual(seq2.getLast(), seq1.getLast(),
              "ArraySequence AssignmentOperator: last");

  seq2.prepend(67);
  assertEqual(
      seq1.getFirst(), 1,
      "ArraySequence AssignmentOperator: deep copy (original not changed)");
}

void testAppendPrependInsertRemove() {
  ArraySequence<int> seq;
  seq.append(10);
  assertEqual(seq.getSize(), 1, "ArraySequence size after append");

  seq.prepend(5);
  assertEqual(seq.getSize(), 2, "ArraySequence size after prepend");

  seq.insertAt(15, 1);
  assertEqual(seq.getSize(), 3, "ArraySequence size after insertAt");

  seq.removeAt(1);
  assertEqual(seq.getSize(), 2, "ArraySequence size after removeAt");

  assertEqual(seq.getFirst(), 5,
              "ArraySequence first element after operations");
  assertEqual(seq.getLast(), 10, "ArraySequence last element after operations");

  try {
    seq.insertAt(15, 555);
    printTestResult("ArraySequence insertAt out of range did not throw", false);
    ++testsFailed;
  } catch (const std::out_of_range &) {
    printTestResult("ArraySequence insertAt out of range throws exception",
                    true);
    ++testsPassed;
  } catch (...) {
    printTestResult("ArraySequence insertAt out of range threw wrong exception",
                    false);
    ++testsFailed;
  }

  try {
    seq.insertAt(15, -555);
    printTestResult("ArraySequence insertAt negative index did not throw",
                    false);
    ++testsFailed;
  } catch (const std::out_of_range &) {
    printTestResult("ArraySequence insertAt negative index throws exception",
                    true);
    ++testsPassed;
  } catch (...) {
    printTestResult(
        "ArraySequence insertAt negative index threw wrong exception", false);
    ++testsFailed;
  }

  try {
    seq.removeAt(555);
    printTestResult("ArraySequence removeAt out of range did not throw", false);
    ++testsFailed;
  } catch (const std::out_of_range &) {
    printTestResult("ArraySequence removeAt out of range throws exception",
                    true);
    ++testsPassed;
  }

  try {
    seq.removeAt(-555);
    printTestResult("ArraySequence removeAt negative index did not throw",
                    false);
    ++testsFailed;
  } catch (const std::out_of_range &) {
    printTestResult("ArraySequence removeAt negative index throws exception",
                    true);
    ++testsPassed;
  }
}

void testGetFirstLast() {
  int source[] = {10, 20, 30, 40};
  ArraySequence<int> seq(source, 4);

  assertEqual(seq.getFirst(), 10, "ArraySequence get: first element");
  assertEqual(seq.getLast(), 40, "ArraySequence get: last element");
  assertEqual(seq.get(2), 30, "ArraySequence get: element with index 2");

  try {
    seq.get(1000);
    printTestResult("ArraySequence get: out of range did not throw", false);
    ++testsFailed;
  } catch (const std::out_of_range &) {
    printTestResult("ArraySequence get: out of range throws exception", true);
    ++testsPassed;
  }

  ArraySequence<int> seqEmpty;

  try {
    seqEmpty.getFirst();
    printTestResult("GetFirst on empty ArraySequence did not throw", false);
    ++testsFailed;
  } catch (const std::out_of_range &) {
    printTestResult("GetFirst on empty ArraySequence throws exception", true);
    ++testsPassed;
  }

  try {
    seqEmpty.getLast();
    printTestResult("GetLast on empty ArraySequence did not throw", false);
    ++testsFailed;
  } catch (const std::out_of_range &) {
    printTestResult("GetLast on empty ArraySequence throws exception", true);
    ++testsPassed;
  }
}

void testSet() {
  int arr[] = {1, 2, 3};
  ArraySequence<int> seq(arr, 3);
  seq.set(42, 1);
  assertEqual(seq.get(1), 42, "ArraySequence: element at index 1 after set");

  try {
    seq.set(67, 555);
    printTestResult("ArraySequence: set out of range did not throw", false);
    ++testsFailed;
  } catch (std::out_of_range &) {
    printTestResult("ArraySequence: set out of range throws exception", true);
    ++testsPassed;
  }

  try {
    seq.set(67, -555);
    printTestResult("ArraySequence: set at negative index did not throw",
                    false);
    ++testsFailed;
  } catch (std::out_of_range &) {
    printTestResult("ArraySequence: set at negative index throws exception",
                    true);
    ++testsPassed;
  }
}

void testGetSubsequence() {
  int arr[] = {1, 2, 3, 4, 5};
  ArraySequence<int> seq(arr, 5);
  Sequence<int> *sub = seq.getSubsequence(1, 3);
  assertEqual(sub->getSize(), 3, "ArraySequence getSubsequence: size");
  assertEqual(sub->getFirst(), 2, "ArraySequence getSubsequence: first");
  assertEqual(sub->getLast(), 4, "ArraySequence getSubsequence: last");
  assertEqual(seq.getSize(), 5,
              "ArraySequence getSubsequence: original unchanged");
  delete sub;
}

void testConcat() {
  int arr1[] = {1, 2};
  int arr2[] = {3, 4};
  ArraySequence<int> seq1(arr1, 2);
  ArraySequence<int> seq2(arr2, 2);
  Sequence<int> *result = seq1.concat(seq2);
  assertEqual(result->getSize(), 4, "ArraySequence concat: size");
  assertEqual(result->getFirst(), 1, "ArraySequence concat: first");
  assertEqual(result->getLast(), 4, "ArraySequence concat: last");
  delete result;
}
} // namespace ArraySequenceTests

namespace ListSequenceTests {

void testDefaultConstructor() {
  ListSequence<int> seq;
  assertEqual(seq.getSize(), 0, "ListSequence default constructor");
}

void testConstructorFromArray() {
  int arr[] = {1, 2, 3};
  ListSequence<int> seq(arr, 3);
  assertEqual(seq.getSize(), 3, "ListSequence constructor from array: size");
  assertEqual(seq.getFirst(), 1,
              "ListSequence constructor from array: getFirst");
  assertEqual(seq.getLast(), 3, "ListSequence constructor from array: getLast");
}

void testCopyConstructor() {
  int arr1[] = {1, 2, 3};
  ListSequence<int> seq1(arr1, 3);
  ListSequence<int> seq2(seq1);

  assertEqual(seq2.getSize(), seq1.getSize(),
              "ListSequence copy constructor: size");
  assertEqual(seq2.getFirst(), seq1.getFirst(),
              "ListSequence copy constructor: first");
  assertEqual(seq2.getLast(), seq1.getLast(),
              "ListSequence copy constructor: last");

  seq2.prepend(67);
  assertEqual(
      seq1.getFirst(), 1,
      "ListSequence copy constructor: deep copy (original not changed)");
}

void testAssignmentOperator() {
  int arr1[] = {1, 2, 3};
  int arr2[] = {4, 5, 6, 7};
  ListSequence<int> seq1(arr1, 3);
  ListSequence<int> seq2(arr2, 4);

  seq2 = seq1;
  assertEqual(seq2.getSize(), seq1.getSize(),
              "ListSequence AssignmentOperator: size");
  assertEqual(seq2.getFirst(), seq1.getFirst(),
              "ListSequence AssignmentOperator: first");
  assertEqual(seq2.getLast(), seq1.getLast(),
              "ListSequence AssignmentOperator: last");

  seq2.prepend(67);
  assertEqual(
      seq1.getFirst(), 1,
      "ListSequence AssignmentOperator: deep copy (original not changed)");
}

void testAppendPrependInsertRemove() {
  ListSequence<int> seq;

  seq.append(10);
  assertEqual(seq.getSize(), 1, "ListSequence size after append");

  seq.prepend(5);
  assertEqual(seq.getSize(), 2, "ListSequence size after prepend");

  seq.insertAt(15, 1);
  assertEqual(seq.getSize(), 3, "ListSequence size after insertAt");

  seq.removeAt(1);
  assertEqual(seq.getSize(), 2, "ListSequence size after removeAt");
  assertEqual(seq.getFirst(), 5, "ListSequence first element after operations");
  assertEqual(seq.getLast(), 10, "ListSequence last element after operations");

  try {
    seq.insertAt(15, 555);
    printTestResult("ListSequence insertAt out of range did not throw", false);
    ++testsFailed;
  } catch (const std::out_of_range &) {
    printTestResult("ListSequence insertAt out of range throws exception",
                    true);
    ++testsPassed;
  } catch (...) {
    printTestResult("ListSequence insertAt out of range threw wrong exception",
                    false);
    ++testsFailed;
  }

  try {
    seq.insertAt(15, -555);
    printTestResult("ListSequence insertAt negative index did not throw",
                    false);
    ++testsFailed;
  } catch (const std::out_of_range &) {
    printTestResult("ListSequence insertAt negative index throws exception",
                    true);
    ++testsPassed;
  } catch (...) {
    printTestResult(
        "ListSequence insertAt negative index threw wrong exception", false);
    ++testsFailed;
  }

  try {
    seq.removeAt(555);
    printTestResult("ListSequence removeAt out of range did not throw", false);
    ++testsFailed;
  } catch (const std::out_of_range &) {
    printTestResult("ListSequence removeAt out of range throws exception",
                    true);
    ++testsPassed;
  }

  try {
    seq.removeAt(-555);
    printTestResult("ListSequence removeAt negative index did not throw",
                    false);
    ++testsFailed;
  } catch (const std::out_of_range &) {
    printTestResult("ListSequence removeAt negative index throws exception",
                    true);
    ++testsPassed;
  }
}

void testGetFirstLast() {
  int source[] = {10, 20, 30, 40};
  ListSequence<int> seq(source, 4);

  assertEqual(seq.getFirst(), 10, "ListSequence get: first element");
  assertEqual(seq.getLast(), 40, "ListSequence get: last element");
  assertEqual(seq.get(2), 30, "ListSequence get: element with index 2");

  try {
    seq.get(1000);
    printTestResult("ListSequence get: out of range did not throw", false);
    ++testsFailed;
  } catch (const std::out_of_range &) {
    printTestResult("ListSequence get: out of range throws exception", true);
    ++testsPassed;
  }

  ListSequence<int> seqEmpty;
  try {
    seqEmpty.getFirst();
    printTestResult("GetFirst on empty ListSequence did not throw", false);
    ++testsFailed;
  } catch (const std::out_of_range &) {
    printTestResult("GetFirst on empty ListSequence throws exception", true);
    ++testsPassed;
  }

  try {
    seqEmpty.getLast();
    printTestResult("GetLast on empty ListSequence did not throw", false);
    ++testsFailed;
  } catch (const std::out_of_range &) {
    printTestResult("GetLast on empty ListSequence throws exception", true);
    ++testsPassed;
  }
}

void testSet() {
  int arr[] = {1, 2, 3};
  ListSequence<int> seq(arr, 3);

  seq.set(67, 0);
  assertEqual(seq.getFirst(), 67, "ListSequence set: first element");

  seq.set(42, 1);
  assertEqual(seq.get(1), 42, "ListSequence set: element at index 1 after set");

  seq.set(69, 2);
  assertEqual(seq.getLast(), 69, "ListSequence set: last element");
}

void testGetSubsequence() {
  int arr[] = {1, 2, 3, 4, 5};
  ListSequence<int> seq(arr, 5);

  Sequence<int> *sub = seq.getSubsequence(1, 3);
  assertEqual(sub->getSize(), 3, "ListSequence getSubsequence: size");
  assertEqual(sub->getFirst(), 2, "ListSequence getSubsequence: first");
  assertEqual(sub->getLast(), 4, "ListSequence getSubsequence: last");
  assertEqual(seq.getSize(), 5,
              "ListSequence getSubsequence: original unchanged");

  delete sub;
}

void testConcat() {
  int arr1[] = {1, 2};
  int arr2[] = {3, 4};
  ListSequence<int> seq1(arr1, 2);
  ListSequence<int> seq2(arr2, 2);

  Sequence<int> *result = seq1.concat(seq2);
  assertEqual(result->getSize(), 4, "ListSequence concat: size");
  assertEqual(result->get(0), 1, "ListSequence concat: first");
  assertEqual(result->get(3), 4, "ListSequence concat: last");

  delete result;
}

} // namespace ListSequenceTests

namespace VectorTests {

void testConstructors() {
  Vector<int> vec1;
  assertEqual(vec1.getSize(), 0, "Vector default constructor: size");

  Vector<int> vec2(3);
  assertEqual(vec2.getSize(), 3, "Vector size constructor: size");

  int arr[] = {1, 2, 3};
  Vector<int> vec3(arr, 3);
  assertEqual(vec3.getSize(), 3, "Vector from array: size");
  assertEqual(vec3.get(0), 1, "Vector from array: first");
  assertEqual(vec3.get(vec3.getSize() - 1), 3, "Vector from array: last");

  ArraySequence<int> seq(arr, 3);
  Vector<int> vec4(seq);
  assertEqual(vec4.getSize(), 3, "Vector from sequence: size");
  assertEqual(vec4.get(0), 1, "Vector from sequence: first");
  assertEqual(vec4.get(vec4.getSize() - 1), 3, "Vector from sequence: last");
}

void testGetSet() {
  Vector<int> vec(3);
  vec.set(10, 0);
  vec.set(20, 1);
  vec.set(30, 2);
  assertEqual(vec.get(0), 10, "Vector get/set: first");
  assertEqual(vec.get(1), 20, "Vector get/set: second");
  assertEqual(vec.get(2), 30, "Vector get/set: third");
}

void testAdd() {
  int arr1[] = {1, 2, 3};
  int arr2[] = {4, 5, 6};
  Vector<int> vec1(arr1, 3);
  Vector<int> vec2(arr2, 3);
  Vector<int> result = vec1.add(vec2);
  assertEqual(result.getSize(), 3, "Vector add: size");
  assertEqual(result.get(0), 5, "Vector add: first");
  assertEqual(result.get(result.getSize() - 1), 9, "Vector add: last");

  int arr3[] = {1, 2, 3, 4};
  Vector<int> vec3(arr3, 4);
  try {
    vec2.add(vec3);
    printTestResult("Vector add: different sizes did not throw", false);
    ++testsFailed;
  } catch (const std::invalid_argument &) {
    printTestResult("Vector add: different sizes sizes throws", true);
    ++testsPassed;
  }
}

void testDotProduct() {
  int arr1[] = {1, 2, 3};
  int arr2[] = {4, 5, 6};
  Vector<int> vec1(arr1, 3);
  Vector<int> vec2(arr2, 3);
  int result = vec1.dotProduct(vec2);
  assertEqual(result, 32, "Vector dotProduct");

  int arr3[] = {1, 2, 3, 4};
  Vector<int> vec3(arr3, 4);
  try {
    vec1.dotProduct(vec3);
    printTestResult("Vector dotProduct: different sizes did not throw", false);
    ++testsFailed;
  } catch (const std::invalid_argument &) {
    printTestResult("Vector dotProduct: different sizes throws", true);
    ++testsPassed;
  }
}

void testScalarMul() {
  int arr[] = {1, 2, 3};
  Vector<int> vec(arr, 3);
  Vector<int> result = vec.scalarMul(2);
  assertEqual(result.getSize(), 3, "Vector scalarMul: size");
  assertEqual(result.get(0), 2, "Vector scalarMul: first");
  assertEqual(result.get(result.getSize() - 1), 6, "Vector scalarMul: last");
}

void testNorm() {
  // int
  int intArr[] = {3, 4};
  Vector<int> intVec(intArr, 2);
  double intNorm = intVec.norm();
  assertEqual(intNorm, 5.0, "Vector int norm (3,4)");

  // complex
  std::complex<double> complexArr[] = {{3, 4}, {0, 0}};
  Vector<std::complex<double>> complexVector(complexArr, 2);
  double complexNorm = complexVector.norm();
  assertEqual(complexNorm, 5.0, "Vector complex norm (3+4i,0)");
}
} // namespace VectorTests

namespace PermutationTests {
void testPreservesElements() {
  int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
  ArraySequence<int> seq(arr, 16);

  int original[16];
  for (int i = 0; i < 16; ++i) {
    original[i] = seq.get(i);
  }

  almostUniformPermutation(&seq, 4);

  assertEqual(seq.getSize(), 16, "Permutation: size unchanged");

  bool allPresent = true;
  for (int i = 0; i < 16; ++i) {
    bool found = false;
    for (int j = 0; j < 16; ++j) {
      if (seq.get(j) == original[i]) {
        found = true;
        break;
      }
    }
    if (!found) {
      allPresent = false;
      break;
    }
  }
  printTestResult("Permutation: all elements preserved", allPresent);
  if (allPresent)
    ++testsPassed;
  else
    ++testsFailed;
}

void testChangesOrder() {
  int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
  ArraySequence<int> seq(arr, 16);

  int original[16];
  for (int i = 0; i < 16; ++i) {
    original[i] = seq.get(i);
  }

  almostUniformPermutation(&seq, 4);

  bool orderChanged = false;
  for (int i = 0; i < 16; ++i) {
    if (seq.get(i) != original[i]) {
      orderChanged = true;
      break;
    }
  }
  printTestResult("Permutation: order changed", orderChanged);
  if (orderChanged)
    ++testsPassed;
  else
    ++testsFailed;
}

void testEmptyAndSingle() {
  ArraySequence<int> empty;
  almostUniformPermutation(&empty, 4);
  assertEqual(empty.getSize(), 0, "Permutation on empty sequence");

  int arr[] = {42};
  ArraySequence<int> single(arr, 1);
  almostUniformPermutation(&single, 4);
  assertEqual(single.getSize(), 1, "Permutation on single element: size");
  assertEqual(single.get(0), 42, "Permutation on single element: value");
}

void testOnListSequence() {
  int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
  ListSequence<int> seq(arr, 16);

  int original[16];
  for (int i = 0; i < 16; ++i) {
    original[i] = seq.get(i);
  }

  almostUniformPermutation(&seq, 4);

  assertEqual(seq.getSize(), 16, "ListSequence permutation: size unchanged");

  bool allPresent = true;
  for (int i = 0; i < 16; ++i) {
    bool found = false;
    for (int j = 0; j < 16; ++j) {
      if (seq.get(j) == original[i]) {
        found = true;
        break;
      }
    }
    if (!found) {
      allPresent = false;
      break;
    }
  }
  printTestResult("ListSequence permutation: elements preserved", allPresent);
  if (allPresent)
    ++testsPassed;
  else
    ++testsFailed;
}
} // namespace PermutationTests

void runAllTests() {
  std::cout << "\n========================================= TESTS "
               "============================================"
            << std::endl;

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

  std::cout << "\nLinkedList:\n" << std::endl;

  {
    using namespace LinkedListTests;
    testDefaultConstructor();
    testConstructorFromArray();
    testCopyConstructor();
    testAssignmentOperator();
    testGetFirstLast();
    testSet();
    testAppendPrepend();
    testInsertAt();
    testRemoveAt();
    testGetSublist();
    testLinkedListConcat();
  }

  std::cout << "\nArraySequence:\n" << std::endl;
  {
    using namespace ArraySequenceTests;
    testDefaultConstructor();
    testConstructorFromArray();
    testCopyConstructor();
    testAssignmentOperator();
    testAppendPrependInsertRemove();
    testGetFirstLast();
    testSet();
    testGetSubsequence();
    testConcat();
  }

  std::cout << "\nListSequence:\n" << std::endl;
  {
    using namespace ListSequenceTests;
    testDefaultConstructor();
    testConstructorFromArray();
    testCopyConstructor();
    testAssignmentOperator();
    testAppendPrependInsertRemove();
    testGetFirstLast();
    testSet();
    testGetSubsequence();
    testConcat();
  }

  std::cout << "\nVector:\n" << std::endl;
  {
    using namespace VectorTests;
    testConstructors();
    testGetSet();
    testAdd();
    testDotProduct();
    testScalarMul();
    testNorm();
  }

  std::cout << "\nPermutations:\n" << std::endl;
  {
    using namespace PermutationTests;
    testPreservesElements();
    testChangesOrder();
    testEmptyAndSingle();
    testOnListSequence();
  }

  std::cout << "\n========================================= RESULTS "
               "==========================================\n"
            << std::endl;
  std::cout << testsPassed << "/" << testsPassed + testsFailed
            << " tests passed." << std::endl;
}