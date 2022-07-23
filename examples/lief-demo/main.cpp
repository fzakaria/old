#include <LIEF/LIEF.hpp>
#include <iostream>
#include <memory>

int main(int argc, char **argv) {
  std::cout << "Abstract Reader" << '\n';
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <binary>" << '\n';
    return -1;
  }

  std::cout << "Parsing with DEBUG level" << '\n';
  std::cout << "========================" << '\n';

  LIEF::logging::set_level(LIEF::logging::LOGGING_LEVEL::LOG_DEBUG);
  std::unique_ptr<const LIEF::Binary> binary_global =
      LIEF::Parser::parse(argv[1]);

  // Disable logger
  LIEF::logging::disable();
  std::unique_ptr<const LIEF::Binary> binary_debug =
      LIEF::Parser::parse(argv[1]);

  return 0;
}