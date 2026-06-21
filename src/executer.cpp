#include "executer.hpp"

#include <sys/wait.h>
#include <unistd.h>

#include <iostream>

#include "builtins.hpp"

#include <fcntl.h>    // Needed for open() 
#include <cstring>    // Needed for strcmp()

void Executer::execute(const std::vector<std::string>& tokens) {
  if (tokens.empty()) return;

  bool is_background = false;
  std::vector<std::string> dup_tokens = tokens;

  if (!dup_tokens.empty() && dup_tokens.back() == "&") {
    is_background = true;
    dup_tokens.pop_back();
  }

  std::vector<const char*> argv;
  for (const std::string& token : dup_tokens) {
    argv.push_back(token.c_str());
  }
  argv.push_back(nullptr);

  if (Builtins::handle(dup_tokens)) {
    return;
  }

  pid_t pid = fork();

  if (pid < 0) {
    std::cerr << tokens[0] << ": failed to execute command" << std::endl;
  } else if (pid == 0) {
    
    for (size_t i = 0; i < argv.size() - 1; ++i) {
      if (argv[i] != nullptr && strcmp(argv[i], ">") == 0) {
        const char* filename = argv[i + 1];
        if (filename == nullptr) {
          std::cerr << "Syntax error: No output file specified" << std::endl;
          exit(1);
        }

        int file_fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (file_fd < 0) {
          perror("open failed");
          exit(1);
        }

        dup2(file_fd, STDOUT_FILENO);
        close(file_fd);

        auto** non_const_argv = const_cast<char**>(argv.data());
        non_const_argv[i] = nullptr;
        break;
      }
    }

    int status = execvp(argv[0], const_cast<char* const*>(argv.data()));

    if (status != 0) {
      std::string msg = "failed to execute command";
      if (errno == ENOENT) {
        msg = "command not found";
      }
      std::cerr << tokens[0] << ": " << msg << std::endl;
      exit(1);
    }
  } else {
    if (!is_background) {
      waitpid(pid, nullptr, 0);
    } else {
      std::cout << "[Process running in background with PID " << pid << "]" << std::endl;
    }
  }
}
