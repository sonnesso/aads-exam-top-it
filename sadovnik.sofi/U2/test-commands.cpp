#include <boost/test/unit_test.hpp>

#include "commands.hpp"
#include "storage.hpp"

#include <sstream>

namespace
{

  void addMeeting(sadovnik::storage_t & storage, std::size_t first,
                  std::size_t second, std::size_t duration)
  {
    sadovnik::ensurePerson(storage, first);
    sadovnik::ensurePerson(storage, second);

    sadovnik::meeting_t meeting;
    meeting.first = first;
    meeting.second = second;
    meeting.duration = duration;
    storage.meetings.pushBack(meeting);
  }

  void addDescribed(sadovnik::storage_t & storage, std::size_t id,
                    const std::string & info)
  {
    sadovnik::person_entry_t person;
    person.id = id;
    person.info = info;
    person.has_info = true;
    storage.persons.pushBack(person);
  }

}

BOOST_AUTO_TEST_CASE(anons_prints_sorted_ids)
{
  sadovnik::storage_t storage;
  sadovnik::initStorage(storage);
  addMeeting(storage, 33, 32, 11);
  addDescribed(storage, 31, "The Agent");

  std::ostringstream out;
  BOOST_TEST(sadovnik::executeCommand(storage, "anons", out));
  BOOST_TEST(out.str() == "32\n33\n");
}

BOOST_AUTO_TEST_CASE(meets_prints_sorted_partner_durations)
{
  sadovnik::storage_t storage;
  sadovnik::initStorage(storage);
  addMeeting(storage, 33, 31, 10);
  addMeeting(storage, 33, 32, 11);
  addMeeting(storage, 31, 32, 99);
  addDescribed(storage, 31, "The Agent");

  std::ostringstream out;
  BOOST_TEST(sadovnik::executeCommand(storage, "meet 33", out));
  BOOST_TEST(out.str() == "31 10\n32 11\n");
}

BOOST_AUTO_TEST_CASE(invalid_command_message)
{
  sadovnik::storage_t storage;
  sadovnik::initStorage(storage);

  std::istringstream in("unknown\n");
  std::ostringstream out;
  sadovnik::processCommands(storage, in, out);
  BOOST_TEST(out.str() == "<INVALID COMMAND>\n");
}

BOOST_AUTO_TEST_CASE(desc_prints_anon_label)
{
  sadovnik::storage_t storage;
  sadovnik::initStorage(storage);
  addMeeting(storage, 33, 31, 10);

  std::ostringstream out;
  BOOST_TEST(sadovnik::executeCommand(storage, "desc 33", out));
  BOOST_TEST(out.str() == "<ANON>\n");
}
