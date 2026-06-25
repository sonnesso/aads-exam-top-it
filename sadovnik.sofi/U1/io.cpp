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

  bool hasId(const sadovnik::List< sadovnik::person_t > & persons, std::size_t id)
  {
    for (sadovnik::LCIter< sadovnik::person_t > it = persons.cbegin();
      it != persons.cend();
      ++it)
    {
      if ((*it).id == id)
      {
        return true;
      }
    }
    return false;
  }

  bool parseId(const std::string & line, std::size_t & pos, std::size_t & id)
  {
    while (pos < line.size() && std::isspace(static_cast<unsigned char>(line[pos])))
    {
      ++pos;
    }

    if (pos >= line.size())
    {
      return false;
    }

    const std::size_t start = pos;
    id = 0;
    while (pos < line.size() && std::isdigit(static_cast<unsigned char>(line[pos])))
    {
      id = id * 10 + static_cast<std::size_t>(line[pos] - '0');
      ++pos;
    }

    if (pos == start)
    {
      return false;
    }

    return true;
  }

  void dropCarriageReturn(std::string & line)
  {
    if (!line.empty() && line.back() == '\r')
    {
      line.pop_back();
    }
  }

}

namespace sadovnik
{

  persons_data_t readPersons(std::istream & in)
  {
    persons_data_t data;
    data.ok_count = 0;
    data.ignored_count = 0;

    std::string line;
    while (std::getline(in, line))
    {
      dropCarriageReturn(line);

      std::size_t pos = 0;
      std::size_t id = 0;
      if (!parseId(line, pos, id))
      {
        ++data.ignored_count;
        continue;
      }

      const std::string info = trimStart(line.substr(pos));
      if (info.empty())
      {
        ++data.ignored_count;
        continue;
      }

      if (hasId(data.persons, id))
      {
        ++data.ignored_count;
        continue;
      }

      person_t person;
      person.id = id;
      person.info = info;
      data.persons.pushBack(person);
      ++data.ok_count;
    }

    return data;
  }

  void writePersons(std::ostream & out, const List< person_t > & persons)
  {
    if (persons.empty())
    {
      out << '\n';
      return;
    }

    for (LCIter< person_t > it = persons.cbegin(); it != persons.cend(); ++it)
    {
      out << (*it).id << ' ' << (*it).info << '\n';
    }
  }

}
