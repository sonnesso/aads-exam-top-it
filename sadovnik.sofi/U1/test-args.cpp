#include <boost/test/unit_test.hpp>

#include "args.hpp"

BOOST_AUTO_TEST_CASE(reject_duplicate_in_argument)
{
  const char * argv[] = {"lab", "in:a", "in:b"};
  sadovnik::paths_t paths;
  const int code = sadovnik::parsePaths(3, const_cast<char **>(argv), paths);
  BOOST_TEST(code == 1);
}

BOOST_AUTO_TEST_CASE(accept_in_and_out_in_any_order)
{
  const char * argv[] = {"lab", "out:out.txt", "in:in.txt"};
  sadovnik::paths_t paths;
  const int code = sadovnik::parsePaths(3, const_cast<char **>(argv), paths);
  BOOST_TEST(code == 0);
  BOOST_TEST(paths.use_in_file);
  BOOST_TEST(paths.use_out_file);
  BOOST_TEST(paths.in_file == "in.txt");
  BOOST_TEST(paths.out_file == "out.txt");
}

BOOST_AUTO_TEST_CASE(reject_unknown_argument)
{
  const char * argv[] = {"lab", "bad:file.txt"};
  sadovnik::paths_t paths;
  const int code = sadovnik::parsePaths(2, const_cast<char **>(argv), paths);
  BOOST_TEST(code == 1);
}

BOOST_AUTO_TEST_CASE(reject_empty_path_after_colon)
{
  const char * argv[] = {"lab", "in:"};
  sadovnik::paths_t paths;
  const int code = sadovnik::parsePaths(2, const_cast<char **>(argv), paths);
  BOOST_TEST(code == 1);
}
