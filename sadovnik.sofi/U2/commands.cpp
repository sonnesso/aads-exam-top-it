#include "commands.hpp"

#include "io.hpp"

#include <cctype>
#include <fstream>
#include <string>
#include <utility>

namespace
{

  const char INVALID_COMMAND[] = "<INVALID COMMAND>";
  const char ANON_LABEL[] = "<ANON>";

  struct meet_view_t
  {
    std::size_t partner;
    std::size_t duration;
  };

  bool isSpace(char symbol)
  {
    return std::isspace(static_cast<unsigned char>(symbol)) != 0;
  }

  std::size_t skipSpaces(const std::string & line, std::size_t pos)
  {
    while (pos < line.size() && isSpace(line[pos]))
    {
      ++pos;
    }
    return pos;
  }

  bool startsWith(const std::string & line, const char * prefix)
  {
    std::size_t pos = 0;
    while (prefix[pos] != '\0')
    {
      if (pos >= line.size() || line[pos] != prefix[pos])
      {
        return false;
      }
      ++pos;
    }
    return true;
  }

  bool parseCommandPrefix(const std::string & line, const char * command, std::size_t & pos)
  {
    if (!startsWith(line, command))
    {
      return false;
    }
    pos = std::string(command).size();
    if (pos < line.size() && !isSpace(line[pos]))
    {
      return false;
    }
    pos = skipSpaces(line, pos);
    return true;
  }

  bool parseSizeT(const std::string & line, std::size_t & pos, std::size_t & value)
  {
    pos = skipSpaces(line, pos);
    if (pos >= line.size() || !std::isdigit(static_cast<unsigned char>(line[pos])))
    {
      return false;
    }

    value = 0;
    while (pos < line.size() && std::isdigit(static_cast<unsigned char>(line[pos])))
    {
      value = value * 10 + static_cast<std::size_t>(line[pos] - '0');
      ++pos;
    }
    return true;
  }

  bool parseIdCommand(const std::string & line, const char * command, std::size_t & id)
  {
    std::size_t pos = 0;
    if (!parseCommandPrefix(line, command, pos))
    {
      return false;
    }
    return parseSizeT(line, pos, id) && skipSpaces(line, pos) == line.size();
  }

  bool parseTwoIdCommand(const std::string & line, const char * command,
                         std::size_t & first, std::size_t & second)
  {
    std::size_t pos = 0;
    if (!parseCommandPrefix(line, command, pos))
    {
      return false;
    }
    if (!parseSizeT(line, pos, first))
    {
      return false;
    }
    return parseSizeT(line, pos, second) && skipSpaces(line, pos) == line.size();
  }

  bool parseTimeIdCommand(const std::string & line, const char * command,
                          std::size_t & time, std::size_t & id)
  {
    std::size_t pos = 0;
    if (!parseCommandPrefix(line, command, pos))
    {
      return false;
    }
    if (!parseSizeT(line, pos, time))
    {
      return false;
    }
    return parseSizeT(line, pos, id) && skipSpaces(line, pos) == line.size();
  }

  bool parseQuotedDescription(const std::string & line, std::size_t pos,
                              std::string & description)
  {
    pos = skipSpaces(line, pos);
    if (pos >= line.size() || line[pos] != '"')
    {
      return false;
    }
    ++pos;

    const std::size_t end = line.find('"', pos);
    if (end == std::string::npos)
    {
      return false;
    }

    description = line.substr(pos, end - pos);
    return skipSpaces(line, end + 1) == line.size() && !description.empty();
  }

  bool parseDescriptionCommand(const std::string & line, const char * command,
                               std::size_t & id, std::string & description)
  {
    std::size_t pos = 0;
    if (!parseCommandPrefix(line, command, pos))
    {
      return false;
    }
    if (!parseSizeT(line, pos, id))
    {
      return false;
    }
    return parseQuotedDescription(line, pos, description);
  }

  bool parseFileCommand(const std::string & line, const char * command, std::string & filename)
  {
    std::size_t pos = 0;
    if (!parseCommandPrefix(line, command, pos))
    {
      return false;
    }
    pos = skipSpaces(line, pos);
    if (pos >= line.size())
    {
      return false;
    }
    filename = line.substr(pos);
    return !filename.empty();
  }

  bool isLessMeetView(const meet_view_t & left, const meet_view_t & right)
  {
    if (left.partner != right.partner)
    {
      return left.partner < right.partner;
    }
    return left.duration < right.duration;
  }

  void insertMeetView(sadovnik::List< meet_view_t > & views, const meet_view_t & view)
  {
    sadovnik::List< meet_view_t > sorted;
    bool inserted = false;

    for (sadovnik::LCIter< meet_view_t > it = views.cbegin(); it != views.cend(); ++it)
    {
      if (!inserted && isLessMeetView(view, *it))
      {
        sorted.pushBack(view);
        inserted = true;
      }
      sorted.pushBack(*it);
    }

    if (!inserted)
    {
      sorted.pushBack(view);
    }

    views = std::move(sorted);
  }

  void insertSizeT(sadovnik::List< std::size_t > & values, std::size_t value)
  {
    sadovnik::List< std::size_t > sorted;
    bool inserted = false;

    for (sadovnik::LCIter< std::size_t > it = values.cbegin(); it != values.cend(); ++it)
    {
      if (!inserted && value < *it)
      {
        sorted.pushBack(value);
        inserted = true;
      }
      sorted.pushBack(*it);
    }

    if (!inserted)
    {
      sorted.pushBack(value);
    }

    values = std::move(sorted);
  }

  void printSizeTList(const sadovnik::List< std::size_t > & values, std::ostream & out)
  {
    if (values.empty())
    {
      out << '\n';
      return;
    }

    for (sadovnik::LCIter< std::size_t > it = values.cbegin(); it != values.cend(); ++it)
    {
      out << *it << '\n';
    }
  }

  void printMeetViews(const sadovnik::List< meet_view_t > & views, std::ostream & out)
  {
    if (views.empty())
    {
      out << '\n';
      return;
    }

    for (sadovnik::LCIter< meet_view_t > it = views.cbegin(); it != views.cend(); ++it)
    {
      out << (*it).partner << ' ' << (*it).duration << '\n';
    }
  }

  std::size_t otherId(const sadovnik::meeting_t & meeting, std::size_t id)
  {
    if (meeting.first == id)
    {
      return meeting.second;
    }
    return meeting.first;
  }

  bool involvesPerson(const sadovnik::meeting_t & meeting, std::size_t id)
  {
    return meeting.first == id || meeting.second == id;
  }

  enum class meet_filter_t
  {
    all,
    less,
    greater
  };

  sadovnik::List< meet_view_t > collectMeetViews(const sadovnik::storage_t & storage,
                                                 std::size_t id,
                                                 meet_filter_t mode,
                                                 std::size_t time)
  {
    sadovnik::List< meet_view_t > views;
    for (sadovnik::LCIter< sadovnik::meeting_t > it = storage.meetings.cbegin();
      it != storage.meetings.cend();
      ++it)
    {
      if (!involvesPerson(*it, id))
      {
        continue;
      }

      const std::size_t duration = (*it).duration;
      if (mode == meet_filter_t::less && !(duration < time))
      {
        continue;
      }
      if (mode == meet_filter_t::greater && !(duration > time))
      {
        continue;
      }

      meet_view_t view;
      view.partner = otherId(*it, id);
      view.duration = duration;
      insertMeetView(views, view);
    }
    return views;
  }

  bool executeAnons(sadovnik::storage_t & storage, std::ostream & out)
  {
    sadovnik::List< std::size_t > anons;
    for (sadovnik::LCIter< sadovnik::person_entry_t > it = storage.persons.cbegin();
      it != storage.persons.cend();
      ++it)
    {
      if (!(*it).has_info)
      {
        insertSizeT(anons, (*it).id);
      }
    }
    printSizeTList(anons, out);
    return true;
  }

  bool executeDesc(sadovnik::storage_t & storage, const std::string & line, std::ostream & out)
  {
    std::size_t id = 0;
    std::string description;
    if (parseDescriptionCommand(line, "desc", id, description))
    {
      if (!sadovnik::hasPerson(storage, id))
      {
        return false;
      }

      sadovnik::person_entry_t * person = sadovnik::findPerson(storage, id);
      person->info = description;
      person->has_info = true;
      return true;
    }

    if (!parseIdCommand(line, "desc", id) || !sadovnik::hasPerson(storage, id))
    {
      return false;
    }

    const sadovnik::person_entry_t * person = sadovnik::findPerson(storage, id);
    if (person->has_info)
    {
      out << person->info << '\n';
    }
    else
    {
      out << ANON_LABEL << '\n';
    }
    return true;
  }

  bool executeRedesc(sadovnik::storage_t & storage, const std::string & line)
  {
    std::size_t id = 0;
    std::string description;
    if (!parseDescriptionCommand(line, "redesc", id, description)
      || !sadovnik::hasPerson(storage, id))
    {
      return false;
    }

    sadovnik::person_entry_t * person = sadovnik::findPerson(storage, id);
    person->info = description;
    person->has_info = true;
    return true;
  }

  bool executeMeets(const sadovnik::storage_t & storage, const std::string & line,
                    std::ostream & out)
  {
    std::size_t id = 0;
    if ((!parseIdCommand(line, "meet", id) && !parseIdCommand(line, "meets", id))
      || !sadovnik::hasPerson(storage, id))
    {
      return false;
    }

    const sadovnik::List< meet_view_t > views =
      collectMeetViews(storage, id, meet_filter_t::all, 0);
    printMeetViews(views, out);
    return true;
  }

  bool executeLess(const sadovnik::storage_t & storage, const std::string & line,
                   std::ostream & out)
  {
    std::size_t time = 0;
    std::size_t id = 0;
    if (!parseTimeIdCommand(line, "less", time, id) || !sadovnik::hasPerson(storage, id))
    {
      return false;
    }

    const sadovnik::List< meet_view_t > views =
      collectMeetViews(storage, id, meet_filter_t::less, time);
    printMeetViews(views, out);
    return true;
  }

  bool executeGreater(const sadovnik::storage_t & storage, const std::string & line,
                      std::ostream & out)
  {
    std::size_t time = 0;
    std::size_t id = 0;
    if (!parseTimeIdCommand(line, "greater", time, id) || !sadovnik::hasPerson(storage, id))
    {
      return false;
    }

    const sadovnik::List< meet_view_t > views =
      collectMeetViews(storage, id, meet_filter_t::greater, time);
    printMeetViews(views, out);
    return true;
  }

  bool executeCommons(const sadovnik::storage_t & storage, const std::string & line,
                      std::ostream & out)
  {
    std::size_t first_id = 0;
    std::size_t second_id = 0;
    if (!parseTwoIdCommand(line, "commons", first_id, second_id)
      || !sadovnik::hasPerson(storage, first_id)
      || !sadovnik::hasPerson(storage, second_id))
    {
      return false;
    }

    sadovnik::List< std::size_t > first_neighbours;
    for (sadovnik::LCIter< sadovnik::meeting_t > it = storage.meetings.cbegin();
      it != storage.meetings.cend();
      ++it)
    {
      if (involvesPerson(*it, first_id))
      {
        insertSizeT(first_neighbours, otherId(*it, first_id));
      }
    }

    sadovnik::List< std::size_t > commons;
    for (sadovnik::LCIter< sadovnik::meeting_t > it = storage.meetings.cbegin();
      it != storage.meetings.cend();
      ++it)
    {
      if (!involvesPerson(*it, second_id))
      {
        continue;
      }

      const std::size_t other = otherId(*it, second_id);
      for (sadovnik::LCIter< std::size_t > neighbour = first_neighbours.cbegin();
        neighbour != first_neighbours.cend();
        ++neighbour)
      {
        if (*neighbour == other)
        {
          bool exists = false;
          for (sadovnik::LCIter< std::size_t > common = commons.cbegin();
            common != commons.cend();
            ++common)
          {
            if (*common == other)
            {
              exists = true;
              break;
            }
          }
          if (!exists)
          {
            insertSizeT(commons, other);
          }
          break;
        }
      }
    }

    printSizeTList(commons, out);
    return true;
  }

  bool executeDeanon(sadovnik::storage_t & storage, const std::string & line)
  {
    std::size_t anon_id = 0;
    std::size_t id = 0;
    if (!parseTwoIdCommand(line, "deanon", anon_id, id)
      || !sadovnik::hasPerson(storage, anon_id)
      || !sadovnik::hasPerson(storage, id)
      || sadovnik::hasDescription(storage, anon_id)
      || !sadovnik::hasDescription(storage, id))
    {
      return false;
    }

    sadovnik::List< sadovnik::meeting_t > updated;
    for (sadovnik::LCIter< sadovnik::meeting_t > it = storage.meetings.cbegin();
      it != storage.meetings.cend();
      ++it)
    {
      sadovnik::meeting_t meeting = *it;
      if (meeting.first == anon_id)
      {
        meeting.first = id;
      }
      if (meeting.second == anon_id)
      {
        meeting.second = id;
      }

      if (meeting.first != meeting.second)
      {
        updated.pushBack(meeting);
      }
    }

    storage.meetings = std::move(updated);
    sadovnik::removePerson(storage, anon_id);
    return true;
  }

  bool executeOutPersons(const sadovnik::storage_t & storage, const std::string & line)
  {
    std::string filename;
    if (!parseFileCommand(line, "out-persons", filename))
    {
      return false;
    }

    std::ofstream file(filename.c_str());
    if (!file)
    {
      return false;
    }

    sadovnik::writeDescribedPersons(file, storage);
    return true;
  }

}

namespace sadovnik
{

  bool executeCommand(storage_t & storage, const std::string & line, std::ostream & out)
  {
    if (line == "anons")
    {
      return executeAnons(storage, out);
    }
    if (startsWith(line, "desc"))
    {
      return executeDesc(storage, line, out);
    }
    if (startsWith(line, "redesc"))
    {
      return executeRedesc(storage, line);
    }
    if (startsWith(line, "meet"))
    {
      return executeMeets(storage, line, out);
    }
    if (startsWith(line, "less"))
    {
      return executeLess(storage, line, out);
    }
    if (startsWith(line, "greater"))
    {
      return executeGreater(storage, line, out);
    }
    if (startsWith(line, "commons"))
    {
      return executeCommons(storage, line, out);
    }
    if (startsWith(line, "deanon"))
    {
      return executeDeanon(storage, line);
    }
    if (startsWith(line, "out-persons"))
    {
      return executeOutPersons(storage, line);
    }
    return false;
  }

  void processCommands(storage_t & storage, std::istream & in, std::ostream & out)
  {
    std::string line;
    while (std::getline(in, line))
    {
      if (!executeCommand(storage, line, out))
      {
        out << INVALID_COMMAND << '\n';
      }
    }
  }

}
