#ifndef ARGS_HPP
#define ARGS_HPP

#include <string>

namespace sadovnik
{

  struct u2_args_t
  {
    bool use_in_file;
    std::string in_file;
    std::string data_file;
  };

  bool parseU2Args(int argc, char * argv[], u2_args_t & args);

}

#endif
