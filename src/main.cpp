#include "ArraySequence.hpp"
#include "ListSequence.hpp"
#include "Permutations.hpp"
#include "Vector.hpp"
#include "tests.h"
#include <iostream>
#include <string>

template <typename T> void printSequence(Sequence<T> *seq) {
  if (!seq) {
    std::cout << "(null)\n";
    return;
  }
  int size = seq->getSize();
  std::cout << "[";
  for (int i = 0; i < size; ++i) {
    std::cout << seq->get(i);
    if (i < size - 1)
      std::cout << ", ";
  }
  std::cout << "]\n";
}

template <typename T> void printVector(Vector<T> *vec) {
  if (!vec) {
    std::cout << "(null)\n";
    return;
  }
  int size = vec->getSize();
  std::cout << "[";
  for (int i = 0; i < size; ++i) {
    std::cout << vec->get(i);
    if (i < size - 1)
      std::cout << ", ";
  }
  std::cout << "]\n";
}

int readInt(const std::string &prompt) {
  int val;
  while (true) {
    std::cout << prompt;
    if (std::cin >> val)
      return val;
    std::cin.clear();
    std::cin.ignore(10000, '\n');
    std::cout << "Invalid input, enter an integer.\n";
  }
}

void sequenceMenu() {
  std::cout << "Choose sequence type:\n";
  std::cout << "1. ArraySequence\n";
  std::cout << "2. ListSequence\n";
  int type = readInt("Type: ");

  Sequence<int> *seq = nullptr;
  if (type == 1)
    seq = new ArraySequence<int>();
  else if (type == 2)
    seq = new ListSequence<int>();
  else {
    std::cout << "Invalid type, creating ArraySequence\n";
    seq = new ArraySequence<int>();
  }

  while (true) {
    std::cout << "\n--- Sequence ---\n";
    std::cout << "Current: ";
    printSequence(seq);
    std::cout << " 1. Append\n";
    std::cout << " 2. Prepend\n";
    std::cout << " 3. Insert at index\n";
    std::cout << " 4. Remove at index\n";
    std::cout << " 5. Get element\n";
    std::cout << " 6. Set element\n";
    std::cout << " 7. Get first / last\n";
    std::cout << " 8. Get subsequence\n";
    std::cout << " 9. Concatenate\n";
    std::cout << "10. Shuffle (almost-uniform permutation)\n";
    std::cout << "11. Back to main menu\n";

    int choice = readInt("Choice: ");

    try {
      if (choice == 1) {
        int val = readInt("Value: ");
        seq->append(val);
      } else if (choice == 2) {
        int val = readInt("Value: ");
        seq->prepend(val);
      } else if (choice == 3) {
        int idx = readInt("Index: ");
        int val = readInt("Value: ");
        seq->insertAt(val, idx);
      } else if (choice == 4) {
        int idx = readInt("Index: ");
        seq->removeAt(idx);
      } else if (choice == 5) {
        int idx = readInt("Index: ");
        std::cout << "Element: " << seq->get(idx) << "\n";
      } else if (choice == 6) {
        int idx = readInt("Index: ");
        int val = readInt("Value: ");
        seq->set(val, idx);
      } else if (choice == 7) {
        std::cout << "First: " << seq->getFirst() << "\n";
        std::cout << "Last:  " << seq->getLast() << "\n";
      } else if (choice == 8) {
        int start = readInt("Start: ");
        int end = readInt("End: ");
        Sequence<int> *sub = seq->getSubsequence(start, end);
        std::cout << "Subsequence: ";
        printSequence(sub);
        delete sub;
      } else if (choice == 9) {
        Sequence<int> *other = nullptr;
        if (type == 1)
          other = new ArraySequence<int>();
        else
          other = new ListSequence<int>();

        int otherSize = readInt("Enter size of the second sequence: ");
        if (otherSize < 0)
          otherSize = 0; 

        std::cout << "Enter " << otherSize << " elements:\n";
        for (int i = 0; i < otherSize; ++i) {
          int val = readInt("Element [" + std::to_string(i) + "]: ");
          other->append(val);
        }

        Sequence<int> *result = seq->concat(*other);
        delete seq;
        delete other;
        seq = result;
        std::cout << "Result: ";
        printSequence(seq);
        
      } else if (choice == 10) {
        int k = readInt("Number of segments (0 for default=4): ");
        if (k <= 0)
          k = 4;
        almostUniformPermutation(seq, k);
        std::cout << "Shuffled: ";
        printSequence(seq);
      } else if (choice == 11) {
        delete seq;
        return;
      } else {
        std::cout << "Unknown command.\n";
      }
    } catch (const std::exception &e) {
      std::cout << "Error: " << e.what() << "\n";
    }
  }
}

void vectorMenu() {
  Vector<int> *vec = new Vector<int>();

  while (true) {
    std::cout << "\n--- Vector ---\n";
    std::cout << "Current: ";
    printVector(vec);
    std::cout << "1. Create new vector from input\n";
    std::cout << "2. Add another vector\n";
    std::cout << "3. Dot product with another vector\n";
    std::cout << "4. Scalar multiplication\n";
    std::cout << "5. Compute norm\n";
    std::cout << "6. Back to main menu\n";

    int choice = readInt("Choice: ");

    try {
      if (choice == 1) {
        int size = readInt("Size: ");
        delete vec;
        vec = new Vector<int>(size);
        for (int i = 0; i < size; ++i) {
          int val = readInt("Element [" + std::to_string(i) + "]: ");
          vec->set(val, i);
        }
      } else if (choice == 2) {
        int size = vec->getSize();
        if (size == 0) {
          std::cout << "Current vector is empty.\n";
          continue;
        }
        std::cout << "Enter " << size << " elements:\n";
        Vector<int> other(size);
        for (int i = 0; i < size; ++i) {
          int val = readInt("Element [" + std::to_string(i) + "]: ");
          other.set(val, i);
        }
        Vector<int> result = vec->add(other);
        std::cout << "Result: ";
        printVector(&result);
      } else if (choice == 3) {
        int size = vec->getSize();
        if (size == 0) {
          std::cout << "Current vector is empty.\n";
          continue;
        }
        std::cout << "Enter " << size << " elements:\n";
        Vector<int> other(size);
        for (int i = 0; i < size; ++i) {
          int val = readInt("Element [" + std::to_string(i) + "]: ");
          other.set(val, i);
        }
        int result = vec->dotProduct(other);
        std::cout << "Dot product: " << result << "\n";
      } else if (choice == 4) {
        int scalar = readInt("Scalar: ");
        Vector<int> result = vec->scalarMul(scalar);
        std::cout << "Result: ";
        printVector(&result);
      } else if (choice == 5) {
        double result = vec->norm();
        std::cout << "Norm: " << result << "\n";
      } else if (choice == 6) {
        delete vec;
        return;
      } else {
        std::cout << "Unknown command.\n";
      }
    } catch (const std::exception &e) {
      std::cout << "Error: " << e.what() << "\n";
    }
  }
}

int main() {
  while (true) {
    std::cout << "\n=== Lab 3: Sequences and Vectors ===\n";
    std::cout << "1. Work with Sequence\n";
    std::cout << "2. Work with Vector\n";
    std::cout << "3. Run all tests\n";
    std::cout << "4. Exit\n";

    int choice = readInt("Choice: ");

    if (choice == 1) {
      sequenceMenu();
    } else if (choice == 2) {
      vectorMenu();
    } else if (choice == 3) {
      runAllTests();
    } else if (choice == 4) {
      break;
    } else {
      std::cout << "Unknown command.\n";
    }
  }
  return 0;
}