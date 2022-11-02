#include <string>

#include "poggles/poggles.hpp"

exported_class::exported_class()
    : m_name {"poggles"}
{
}

auto exported_class::name() const -> const char*
{
  return m_name.c_str();
}
