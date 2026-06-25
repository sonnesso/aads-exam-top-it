#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "storage.hpp"

#include <iosfwd>
#include <string>

namespace sadovnik
{

  bool executeCommand(storage_t & storage, const std::string & line, std::ostream & out);
  void processCommands(storage_t & storage, std::istream & in, std::ostream & out);

}

#endif
