#include "io.hpp"

#include <cctype>
#include <iostream>
#include <string>

namespace
{

  std::string trimStart(const std::string & text)
  {
    std::size_t pos = 0;
    while (pos < text.size() && std::isspace(static_cast<unsigned char>(text[pos])))
    {
      ++pos;
    }
    return text.substr(pos);
  }

}

namespace sadovnik
{

  List< person_t > readPersons(std::istream & in)
  {
    List< person_t > persons;
    std::size_t id = 0;

    while (in >> id)
    {
      std::string line;
      if (!std::getline(in, line))
      {
        break;
      }

      person_t person;
      person.id = id;
      person.info = trimStart(line);
      persons.pushBack(person);
    }

    return persons;
  }

  void writePersons(std::ostream & out, const List< person_t > & persons)
  {
    for (LCIter< person_t > it = persons.cbegin(); it != persons.cend(); ++it)
    {
      out << (*it).id << ' ' << (*it).info << '\n';
    }
  }

}
