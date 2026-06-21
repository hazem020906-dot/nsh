#pragma once

#include <string>
#include <vector>

class Builtins {
public:
    static bool handle(const std::vector<std::string>& tokens);
    static void add_to_history(const std::string& command_line);
};
