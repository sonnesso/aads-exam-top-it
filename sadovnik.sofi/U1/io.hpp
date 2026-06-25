#ifndef IO_HPP
#define IO_HPP

#include "person.hpp"

#include <list.hpp>

#include <iosfwd>

namespace sadovnik
{

  List< person_t > readPersons(std::istream & in);
  void writePersons(std::ostream & out, const List< person_t > & persons);

}

#endif
