#ifndef PERSON_HPP
#define PERSON_HPP

#include <cstddef>
#include <string>

namespace sadovnik
{

  struct person_entry_t
  {
    std::size_t id;
    std::string info;
    bool has_info;
  };

}

#endif
