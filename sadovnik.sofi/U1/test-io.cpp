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

  sadovnik::LCIter< sadovnik::person_t > it = data.persons.cbegin();
  BOOST_TEST((*it).id == 100);
  ++it;
  BOOST_TEST((*it).id == 23);
  ++it;
  BOOST_TEST((*it).id == 31);
  ++it;
  BOOST_TEST((*it).id == 56);
}

BOOST_AUTO_TEST_CASE(example_full_output)
{
  std::istringstream in(
    "100 Sneaky person\n"
    "23 Deadman\n"
    "31 The Agent\n"
    "56 \"Scarface\"\n"
    "99\n"
    "31 Mr. Bond\n");

  const sadovnik::persons_data_t data = sadovnik::readPersons(in);
  std::ostringstream out;
  sadovnik::writePersons(out, data.persons);

  BOOST_TEST(out.str() ==
    "100 Sneaky person\n"
    "23 Deadman\n"
    "31 The Agent\n"
    "56 \"Scarface\"\n");
}

BOOST_AUTO_TEST_CASE(empty_input_zero_counts)
{
  std::istringstream in("");
  const sadovnik::persons_data_t data = sadovnik::readPersons(in);

  BOOST_TEST(data.ok_count == 0);
  BOOST_TEST(data.ignored_count == 0);
  BOOST_TEST(data.persons.empty());

  std::ostringstream out;
  sadovnik::writePersons(out, data.persons);
  BOOST_TEST(out.str() == "\n");
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

BOOST_AUTO_TEST_CASE(read_id_without_space_before_info)
{
  std::istringstream in("31The Agent\n");
  const sadovnik::persons_data_t data = sadovnik::readPersons(in);

  BOOST_TEST(data.ok_count == 1);
  BOOST_TEST(data.ignored_count == 0);
  BOOST_TEST((*data.persons.cbegin()).id == 31);
  BOOST_TEST((*data.persons.cbegin()).info == "The Agent");
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

BOOST_AUTO_TEST_CASE(read_persons_skips_empty_lines)
{
  std::istringstream in(
    "\n"
    " \t\n"
    "31 Mr. Bond\n"
    "\n");

  const sadovnik::persons_data_t data = sadovnik::readPersons(in);

  BOOST_TEST(data.ok_count == 1);
  BOOST_TEST(data.ignored_count == 0);
  BOOST_TEST(data.persons.size() == 1);
  BOOST_TEST((*data.persons.cbegin()).id == 31);
}
