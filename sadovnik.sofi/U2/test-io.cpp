#include <boost/test/unit_test.hpp>

#include "io.hpp"
#include "storage.hpp"

#include <sstream>

BOOST_AUTO_TEST_CASE(read_meetings_skip_self_meeting)
{
  std::istringstream in(
    "33 41 10\n"
    "33 33 100\n");

  sadovnik::storage_t storage;
  sadovnik::initStorage(storage);

  BOOST_TEST(sadovnik::readMeetingsFile(in, storage));
  BOOST_TEST(storage.meetings.size() == 1);
  BOOST_TEST(storage.persons.size() == 2);
}

BOOST_AUTO_TEST_CASE(read_meetings_reject_bad_line)
{
  std::istringstream in("33 41 bad\n");

  sadovnik::storage_t storage;
  sadovnik::initStorage(storage);

  BOOST_TEST(!sadovnik::readMeetingsFile(in, storage));
}

BOOST_AUTO_TEST_CASE(read_persons_file_keeps_valid_records)
{
  std::istringstream in(
    "31 The Agent\n"
    "99\n"
    "31 Mr. Bond\n");

  sadovnik::storage_t storage;
  sadovnik::initStorage(storage);
  sadovnik::readPersonsFile(in, storage);

  BOOST_TEST(storage.persons.size() == 1);
  BOOST_TEST((*storage.persons.cbegin()).id == 31);
  BOOST_TEST((*storage.persons.cbegin()).info == "The Agent");
  BOOST_TEST((*storage.persons.cbegin()).has_info);
}
