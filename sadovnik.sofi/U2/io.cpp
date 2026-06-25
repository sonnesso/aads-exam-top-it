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

  void dropCarriageReturn(std::string & line)
  {
    if (!line.empty() && line.back() == '\r')
    {
      line.pop_back();
    }
  }

  bool isBlankLine(const std::string & line)
  {
    std::size_t pos = 0;
    while (pos < line.size() && std::isspace(static_cast<unsigned char>(line[pos])))
    {
      ++pos;
    }
    return pos >= line.size();
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

    return pos > start;
  }

  bool hasId(const sadovnik::List< sadovnik::person_entry_t > & persons, std::size_t id)
  {
    for (sadovnik::LCIter< sadovnik::person_entry_t > it = persons.cbegin();
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

  bool parseMeetingLine(const std::string & line, sadovnik::meeting_t & meeting)
  {
    std::size_t pos = 0;
    if (!parseId(line, pos, meeting.first))
    {
      return false;
    }
    if (!parseId(line, pos, meeting.second))
    {
      return false;
    }
    if (!parseId(line, pos, meeting.duration))
    {
      return false;
    }

    const std::string tail = trimStart(line.substr(pos));
    return tail.empty();
  }

}

namespace sadovnik
{

  void readPersonsFile(std::istream & in, storage_t & storage)
  {
    std::string line;
    while (std::getline(in, line))
    {
      dropCarriageReturn(line);

      if (isBlankLine(line))
      {
        continue;
      }

      std::size_t pos = 0;
      std::size_t id = 0;
      if (!parseId(line, pos, id))
      {
        continue;
      }

      const std::string info = trimStart(line.substr(pos));
      if (info.empty() || hasId(storage.persons, id))
      {
        continue;
      }

      person_entry_t person;
      person.id = id;
      person.info = info;
      person.has_info = true;
      storage.persons.pushBack(person);
    }
  }

  bool readMeetingsFile(std::istream & in, storage_t & storage)
  {
    std::string line;
    while (std::getline(in, line))
    {
      dropCarriageReturn(line);

      if (isBlankLine(line))
      {
        continue;
      }

      meeting_t meeting;
      if (!parseMeetingLine(line, meeting))
      {
        return false;
      }

      if (meeting.first == meeting.second)
      {
        continue;
      }

      ensurePerson(storage, meeting.first);
      ensurePerson(storage, meeting.second);
      storage.meetings.pushBack(meeting);
    }

    return true;
  }

  void writeDescribedPersons(std::ostream & out, const storage_t & storage)
  {
    bool wrote = false;
    for (LCIter< person_entry_t > it = storage.persons.cbegin();
      it != storage.persons.cend();
      ++it)
    {
      if (!(*it).has_info)
      {
        continue;
      }

      if (wrote)
      {
        out << '\n';
      }
      out << (*it).id << ' ' << (*it).info;
      wrote = true;
    }
    out << '\n';
  }

}
