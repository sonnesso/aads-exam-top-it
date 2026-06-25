#include "io.hpp"

#include <iostream>

int main()
{
  using namespace sadovnik;

  const persons_data_t data = readPersons(std::cin);
  writePersons(std::cout, data.persons);
  std::cerr << data.ok_count << ' ' << data.ignored_count << '\n';
  return 0;
}
