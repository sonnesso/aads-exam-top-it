#ifndef IO_HPP
#define IO_HPP

#include "person.hpp"

#include <list.hpp>

#include <cstddef>
#include <iosfwd>

namespace sadovnik
{

  struct persons_data_t
  {
    List< person_t > persons;
    std::size_t ok_count;
    std::size_t ignored_count;
  };

  persons_data_t readPersons(std::istream & in);
  void writePersons(std::ostream & out, const List< person_t > & persons);

}

#endif
