#include "tests.h"
#include <iostream>
#include <string>

int main() {
  std::string command;
  std::cout << "Enter 'test' to run tests, 'exit' to quit.\n";

  while (true) {
    std::cout << "> ";
    std::cin >> command;

    if (command == "test") {
      runAllTests();
    } else if (command == "exit") {
      break;
    } else {
      std::cout << "Unknown command. Use 'test' or 'exit'.\n";
    }
  }
  return 0;
}