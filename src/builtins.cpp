#include "builtins.hpp"
#include <unistd.h>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>

// This is our global history logbook list
static std::vector<std::string> shell_history;

// This logs any command entered into the logbook
void Builtins::add_to_history(const std::string& command_line) {
  if (!command_line.empty()) {
    shell_history.push_back(command_line);
  }
}

bool Builtins::handle(const std::vector<std::string>& tokens) {
  if (tokens.empty()) return false;

  // --- TASK 5: Catch the 'history' command ---
  if (tokens[0] == "history") {
    for (size_t i = 0; i < shell_history.size(); ++i) {
      std::cout << " " << (i + 1) << "  " << shell_history[i] << std::endl;
    }
    return true;
  }

  if (tokens[0] == "exit") {
    std::exit(EXIT_SUCCESS);
  } else if (tokens[0] == "cd") {
    size_t argc = tokens.size();
    std::string path = argc == 1 ? "~" : tokens[1];

    if (argc > 2) {
      std::cerr << "cd: too many arguments" << std::endl;
    } else {
      int status = chdir(path == "~" ? std::getenv("HOME") : path.c_str());

      if (status != 0) {
        std::string msg = "failed to change directory";

        if (errno == ENOENT) {
          msg = "no such file or directory";
        } else if (errno == EACCES) {
          msg = "permission denied";
        }

        std::cerr << "cd: " << msg << ": " << path << std::endl;
      }
    }

    return true;
  }

  return false;
}
