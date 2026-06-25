#include <boost/test/unit_test.hpp>

#include "io.hpp"

#include <sstream>

BOOST_AUTO_TEST_CASE(read_persons_from_example)
{
  std::istringstream in(
    "100 Sneaky person\n"
    "23 Deadman\n"
    "31 The Agent\n"
    "56 \"Scarface\"\n"
    "99\n"
    "31 Mr. Bond\n");

  const sadovnik::persons_data_t data = sadovnik::readPersons(in);

  BOOST_TEST(data.ok_count == 4);
  BOOST_TEST(data.ignored_count == 2);
  BOOST_TEST(data.persons.size() == 4);
}

BOOST_AUTO_TEST_CASE(write_persons_one_space)
{
  sadovnik::List< sadovnik::person_t > persons;
  sadovnik::person_t person;
  person.id = 31;
  person.info = "The Agent";
  persons.pushBack(person);

  std::ostringstream out;
  sadovnik::writePersons(out, persons);

  BOOST_TEST(out.str() == "31 The Agent\n");
}

BOOST_AUTO_TEST_CASE(ignore_id_stuck_to_text)
{
  std::istringstream in("100 ok\n100bad no\n");
  const sadovnik::persons_data_t data = sadovnik::readPersons(in);

  BOOST_TEST(data.ok_count == 1);
  BOOST_TEST(data.ignored_count == 1);
}

BOOST_AUTO_TEST_CASE(read_line_with_carriage_return)
{
  std::istringstream in("31 Agent\r\n");
  const sadovnik::persons_data_t data = sadovnik::readPersons(in);

  BOOST_TEST(data.ok_count == 1);
  BOOST_TEST(data.ignored_count == 0);
  BOOST_TEST(data.persons.size() == 1);
  BOOST_TEST((*data.persons.cbegin()).info == "Agent");
}
