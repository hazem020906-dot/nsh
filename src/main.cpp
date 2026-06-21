#include <iostream>
#include <string>
#include <vector>
#include "executer.hpp"
#include "parser.hpp"
#include "builtins.hpp"

int main() {
  while (true) {
    std::cout << "nsh> ";
    std::string input;
    std::getline(std::cin, input);

    if (!input.empty()) {
      Builtins::add_to_history(input);
    }

    if (input.empty()) {
      continue;
    }

    std::vector<std::string> tokens = Parser::parseInput(input);
    Executer::execute(tokens);
  }
}
