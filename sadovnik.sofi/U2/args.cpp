#include "args.hpp"

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

  bool parseU2Args(int argc, char * argv[], u2_args_t & args)
  {
    args.use_in_file = false;
    args.in_file.clear();
    args.data_file.clear();

    if (argc < 2 || argc > 3)
    {
      return false;
    }

    bool has_data = false;

    for (int i = 1; i < argc; ++i)
    {
      const std::string arg = argv[i];
      if (hasPrefix(arg, "in:"))
      {
        if (args.use_in_file)
        {
          return false;
        }
        args.in_file = takePath(arg, "in:");
        if (args.in_file.empty())
        {
          return false;
        }
        args.use_in_file = true;
      }
      else if (hasPrefix(arg, "data:"))
      {
        if (has_data)
        {
          return false;
        }
        args.data_file = takePath(arg, "data:");
        if (args.data_file.empty())
        {
          return false;
        }
        has_data = true;
      }
      else
      {
        return false;
      }
    }

    return has_data;
  }

}
