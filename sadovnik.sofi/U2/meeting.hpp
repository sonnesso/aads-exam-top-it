#ifndef MEETING_HPP
#define MEETING_HPP

#include <cstddef>

namespace sadovnik
{

  struct meeting_t
  {
    std::size_t first;
    std::size_t second;
    std::size_t duration;
  };

}

#endif
