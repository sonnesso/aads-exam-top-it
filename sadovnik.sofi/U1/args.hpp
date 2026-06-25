#ifndef ARGS_HPP
#define ARGS_HPP

#include <string>

namespace sadovnik
{

  struct paths_t
  {
    std::string in_file;
    std::string out_file;
    bool use_in_file;
    bool use_out_file;
  };

  int parsePaths(int argc, char * argv[], paths_t & paths);

}

#endif
