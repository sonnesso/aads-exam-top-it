#include <boost/test/unit_test.hpp>

#include "args.hpp"

BOOST_AUTO_TEST_CASE(accept_data_argument_only)
{
  const char * argv[] = {"lab", "data:meets.txt"};
  sadovnik::u2_args_t args;
  BOOST_TEST(sadovnik::parseU2Args(2, const_cast<char **>(argv), args));
  BOOST_TEST(!args.use_in_file);
  BOOST_TEST(args.data_file == "meets.txt");
}

BOOST_AUTO_TEST_CASE(accept_in_and_data_in_any_order)
{
  const char * argv[] = {"lab", "in:persons.txt", "data:meets.txt"};
  sadovnik::u2_args_t args;
  BOOST_TEST(sadovnik::parseU2Args(3, const_cast<char **>(argv), args));
  BOOST_TEST(args.use_in_file);
  BOOST_TEST(args.in_file == "persons.txt");
  BOOST_TEST(args.data_file == "meets.txt");
}

BOOST_AUTO_TEST_CASE(reject_missing_data_argument)
{
  const char * argv[] = {"lab", "in:persons.txt"};
  sadovnik::u2_args_t args;
  BOOST_TEST(!sadovnik::parseU2Args(2, const_cast<char **>(argv), args));
}

BOOST_AUTO_TEST_CASE(reject_unknown_argument)
{
  const char * argv[] = {"lab", "bad:file.txt"};
  sadovnik::u2_args_t args;
  BOOST_TEST(!sadovnik::parseU2Args(2, const_cast<char **>(argv), args));
}
