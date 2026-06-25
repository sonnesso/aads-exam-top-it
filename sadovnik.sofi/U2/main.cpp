#include "args.hpp"
#include "commands.hpp"
#include "io.hpp"
#include "storage.hpp"

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

  u2_args_t args;
  if (!parseU2Args(argc, argv, args))
  {
    std::cerr << "Invalid arguments\n";
    return 1;
  }

  storage_t storage;
  initStorage(storage);

  if (args.use_in_file)
  {
    std::ifstream in_file(args.in_file.c_str());
    if (!in_file)
    {
      std::cerr << "File open error\n";
      return 2;
    }
    readPersonsFile(in_file, storage);
  }

  std::ifstream data_file(args.data_file.c_str());
  if (!data_file)
  {
    std::cerr << "File open error\n";
    return 2;
  }

  if (!readMeetingsFile(data_file, storage))
  {
    std::cerr << "Invalid meeting data\n";
    return 3;
  }

  processCommands(storage, std::cin, std::cout);
  return 0;
}
