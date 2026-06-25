#include "args.hpp"

#include <iostream>
#include <string>

namespace
{

  bool hasPrefix(const std::string & text, const std::string & prefix)
  {
    return text.size() >= prefix.size()
      && text.compare(0, prefix.size(), prefix) == 0;
  }

  std::string takePath(const std::string & arg, const std::string & prefix)
  {
    return arg.substr(prefix.size());
  }

}

namespace sadovnik
{

  int parsePaths(int argc, char * argv[], paths_t & paths)
  {
    paths.in_file.clear();
    paths.out_file.clear();
    paths.use_in_file = false;
    paths.use_out_file = false;

    if (argc > 3)
    {
      std::cerr << "invalid arguments\n";
      return 1;
    }

    for (int i = 1; i < argc; ++i)
    {
      const std::string arg = argv[i];
      if (hasPrefix(arg, "in:"))
      {
        if (paths.use_in_file)
        {
          std::cerr << "invalid arguments\n";
          return 1;
        }
        paths.in_file = takePath(arg, "in:");
        if (paths.in_file.empty())
        {
          std::cerr << "invalid arguments\n";
          return 1;
        }
        paths.use_in_file = true;
      }
      else if (hasPrefix(arg, "out:"))
      {
        if (paths.use_out_file)
        {
          std::cerr << "invalid arguments\n";
          return 1;
        }
        paths.out_file = takePath(arg, "out:");
        if (paths.out_file.empty())
        {
          std::cerr << "invalid arguments\n";
          return 1;
        }
        paths.use_out_file = true;
      }
      else
      {
        std::cerr << "invalid arguments\n";
        return 1;
      }
    }

    return 0;
  }

}
