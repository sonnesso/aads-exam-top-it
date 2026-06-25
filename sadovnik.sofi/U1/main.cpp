#include "args.hpp"
#include "io.hpp"

#include <fstream>
#include <iostream>

int main(int argc, char * argv[])
{
  using namespace sadovnik;

  if (argc > 3)
  {
    std::cerr << "Too many arguments\n";
    return 0;
  }

  paths_t paths;
  const int parse_code = parsePaths(argc, argv, paths);
  if (parse_code != 0)
  {
    return parse_code;
  }

  std::ifstream in_file;
  std::istream * in = &std::cin;
  if (paths.use_in_file)
  {
    in_file.open(paths.in_file);
    if (!in_file)
    {
      std::cerr << "Cannot open input file\n";
      return 2;
    }
    in = &in_file;
  }

  const persons_data_t data = readPersons(*in);
  if (paths.use_in_file)
  {
    in_file.close();
  }

  std::ofstream out_file;
  std::ostream * out = &std::cout;
  if (paths.use_out_file)
  {
    out_file.open(paths.out_file);
    if (!out_file)
    {
      std::cerr << "Cannot open output file\n";
      return 2;
    }
    out = &out_file;
  }

  writePersons(*out, data.persons);
  std::cerr << data.ok_count << ' ' << data.ignored_count << '\n';
  return 0;
}
