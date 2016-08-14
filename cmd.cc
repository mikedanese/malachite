#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cstring>

#include "ast.h"
#include "serp.h"
#include "vm.h"

int main(const int argc, const char** argv) {
  try {
    serpentine::Scanner scanner(&std::cin);
    serpentine::Parser parser(scanner);

    if (parser.parse()) {
      std::cerr << "Parse failed!!\n";
      return 1;
    }

    return 0;
  } catch (const std::exception& ex) {
    std::cerr << "Something went terribly wrong!\n";
    throw ex;
  }
}
