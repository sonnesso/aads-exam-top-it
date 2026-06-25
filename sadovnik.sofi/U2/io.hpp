#ifndef IO_HPP
#define IO_HPP

#include "meeting.hpp"
#include "person.hpp"
#include "storage.hpp"

#include <list.hpp>

#include <iosfwd>

namespace sadovnik
{

  void readPersonsFile(std::istream & in, storage_t & storage);
  bool readMeetingsFile(std::istream & in, storage_t & storage);
  void writeDescribedPersons(std::ostream & out, const storage_t & storage);

}

#endif
