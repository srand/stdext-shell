#ifndef __STDEXT_SHELL__
#define __STDEXT_SHELL__

#include <functional>
#include <map>
#include <string>
#include <vector>

namespace stdext {

using string_list = std::vector<std::string>;
using shell_command = std::function<bool(const string_list &)>;
using shell_command_map = std::map<std::string, shell_command>;

class shell {
  shell_command_map _commands;

public:
  shell();

  void add(const std::string &name, const shell_command &func);
  bool run();

private:
  bool readline(std::string &);
};

} // stdext

#endif // __STDEXT_SHELL__
