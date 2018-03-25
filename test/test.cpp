#include <iostream>
#include <stdext/shell.h>

int main(int argc, char *argv[]) {
  stdext::shell shell;

  shell.add("hello", [](const stdext::string_list &) {
    std::cout << "Hello world" << std::endl;
    return true;
  });
  shell.add("quit", [](const stdext::string_list &) { return false; });
  shell.run();

  return EXIT_SUCCESS;
}
