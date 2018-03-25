#include "stdext/shell.h"
#include <iostream>
#include <sstream>

#ifdef __linux
#include <signal.h>
#include <termios.h>

namespace {
struct termios old_term, new_term;

void initialize_termios(int echo) {
  tcgetattr(0, &old_term);
  new_term = old_term;
  new_term.c_lflag &= ~ICANON;
  new_term.c_lflag &= echo ? ECHO : ~ECHO;
  tcsetattr(0, TCSANOW, &new_term);
}

void reset_termios(void) { tcsetattr(0, TCSANOW, &old_term); }

char getch(void) {
  char ch;
  initialize_termios(0);
  ch = getchar();
  reset_termios();
  return ch;
}

} // anon namespace
#endif // __linux

namespace stdext {

shell::shell() {}

void shell::add(const std::string &name, const stdext::shell_command &func) {
  _commands[name] = func;
}

bool shell::run() {
  std::string line;

  while (readline(line)) {
    std::istringstream is(line);
    std::string cmd, arg;
    stdext::string_list args;

    is >> cmd;
    while (is >> arg) {
      args.push_back(arg);
    }

    auto it = _commands.find(cmd);
    if (it != _commands.end()) {
      if (!it->second(args)) {
        return false;
      }
    } else if (!cmd.empty()) {
      printf("%s: unknown command\n", cmd.c_str());
    }
  }
  return false;
}

bool shell::readline(std::string &line) {
  int c;
  line.clear();

  printf("$ ");

  while ((c = getch()) != -1) {
    switch (c) {
    case 27: // Up-arrow
      getch();
      getch();
      break;

    case 127:
    case '\b':
      if (line.empty())
        break;
      printf("\b \b");
      line.erase(line.size() - 1);
      break;
    case 4:
      return false;
    case '\n':
    case '\r':
      printf("\n");
      return true;
    case '\t':
      if (line.empty()) {
        printf("\n");
        for (auto cmd : _commands) {
          printf("%s ", cmd.first.c_str());
        }
        printf("\n$ %s", line.c_str());
      } else {
        if (line.find(" ") != line.npos)
          break;

        stdext::string_list args;
        for (auto cmd : _commands) {
          if (cmd.first.find(line) != std::string::npos) {
            args.push_back(cmd.first);
          }
        }

        if (args.size() == 1) {
          line = args[0];
          printf("\r$ %s", line.c_str());
        } else if (args.size() > 1) {
          printf("\n");
          for (auto arg : args) {
            printf("%s ", arg.c_str());
          }
          printf("\n$ %s", line.c_str());
        }
      }
      break;
    default:
      line += c;
      printf("%c", c);
    }
  }
  return c != -1;
}

} // stdext
