#include <string>

#include "poggles/poggles.hpp"

auto main() -> int
{
  auto const exported = exported_class {};

  return std::string("poggles") == exported.name() ? 0 : 1;
}
